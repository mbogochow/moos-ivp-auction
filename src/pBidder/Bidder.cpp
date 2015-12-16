/**
 * @author Mike Bogochow
 * @version 2.4.0, Dec 8, 2015
 *
 * @file Bidder.cpp
 *
 * Bidder class implementation
 */

#include "Bidder.h"
#include "../lib_graphs/Graph.h"
#include "../lib_graphs/Subgraph.h"
#include "../lib_graphs/SpanningTree.h"
#include "../lib_graphs/Path.h"
//#include "../lib_graphs/defs.h"
#include "../lib_auction/AuctionDefs.h"
#include "../lib_auction/DebugPrinter.h"

#include "MBUtils.h"

#include <boost/lexical_cast.hpp>

#include <algorithm> // std::remove
#include <chrono>
#include <thread>

Bidder::Bidder(void)
{
  m_iterations = 0;

//  g = new Graph(__edges, __num_edges, __weights, __num_nodes);
//
//  // TODO add me to the graph
//
//  allocated.reserve(__num_nodes);
//  unallocated.reserve(__num_nodes);
//
//  for (Vertex i = 0; i < __num_nodes; i++)
//    unallocated.push_back(i);
  g = nullptr;

  rtc = 0;

  roundNumber = 0;
  roundUpdated = false;
  winnerUpdated = false;

  id = -1;
}

Bidder::~Bidder(void)
{
  delete g;
}

bool
Bidder::OnNewMail(MOOSMSG_LIST &NewMail)
{
  bool ret = AuctionMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::reverse_iterator p;
  for(p = NewMail.rbegin(); p != NewMail.rend(); p++)
  {
    CMOOSMsg &msg = *p;
    std::string key   = msg.GetKey();
MOOSTrace("key=%s, g==nullptr=%s\n", key.c_str(), boost::lexical_cast<std::string>(g==nullptr).c_str());
    if (key == MVAR_BID_TARGETS && g == nullptr) // ignore if already have g
    {
      std::string sTargets = msg.GetString();
      pathFromString(sTargets, targets);
      size_t numTargets = targets.size();
      std::vector<Edge> edges;
      std::vector<mbogo_weight_t> weights;
      connectEdges(targets, edges, weights);

      g = new Graph(edges.data(), edges.size(), weights.data(), numTargets);

      // TODO add me to the graph

      allocated.reserve(numTargets);
      unallocated.reserve(numTargets);

      for (Vertex i = 0; i < numTargets; i++)
        unallocated.push_back(i);
    }
    else if (key == MVAR_BID_START)
    {
      size_t num = boost::lexical_cast<size_t>(msg.GetString());
      if (num > roundNumber) // ignore duplicate messages
      {
        if (num != roundNumber + 1)
        {
          MOOSTrace("WARNING: received round number %i while on round %i",
              num, roundNumber);
        }

        roundUpdated = true;
        roundNumber = num;
        dp.dprintf(LVL_MIN_VERB, "Got %s mail: %i\n", MVAR_BID_START.c_str(),
            num);
      }
    }
    else if (key == MVAR_BID_WINNER)
    {
      winnerUpdated = true;
      winningBid = winningBidFromString(msg.GetString());
      dp.dprintf(LVL_MIN_VERB, "Got %s mail: %s\n", MVAR_BID_WINNER.c_str(),
          winningBid);
    }
  }

  return ret;
}

bool
Bidder::Iterate(void)
{
  bool ret = AuctionMOOSApp::Iterate();

  if (g != nullptr)
  {
    if (winnerUpdated)
    {
      if (winningBid.winner == id)
      {
        allocated.push_back(winningBid.target);
        rtc += winningBid.bid;
      }
      unallocated.erase(
          std::remove(unallocated.begin(), unallocated.end(),
              winningBid.target), unallocated.end());
      winnerUpdated = false;
    }

    dp.dprintf(LVL_MAX_VERB, "roundNum <= numNodes (%lu <= %lu)?\n", roundNumber, g->getNumVertices());
    if (roundNumber <= g->getNumVertices())
    {
      if (roundUpdated)
      {
        performBiddingRound();
        roundUpdated = false;
      }
    }
    else
    {
      assert(unallocated.size() == 0);

      performFinalCalc();

      // Exit pBidder
      doNotify("EXITED_NORMALLY", "pBidder");
      // Ensure all data makes it to the MOOSDB
      std::this_thread::sleep_for(std::chrono::seconds(10));
      exit(0);
    }
  }

  return ret;
}

void
Bidder::performBiddingRound(void)
{
  // Bidding round
  Bid currentBid = std::make_pair(MAX_VERTEX, MAX_WEIGHT);

  Graph *sub;

  // Iterate through unallocated nodes to find bid
  for (std::vector<Vertex>::iterator t = unallocated.begin();
      t != unallocated.end(); t++)
  {
    Path *path;
    SpanningTree *tree;
    mbogo_weight_t cost;
    mbogo_weight_t bid;
    std::vector<Vertex> possibleAllocation;

    possibleAllocation.reserve(allocated.size() + 1);
    possibleAllocation = allocated;
    dp.dprintf(LVL_BID, "Adding %s for possible allocation\n",
        boost::lexical_cast<std::string>(*t).c_str());
    possibleAllocation.push_back(*t);

    // Catch simple cases for efficiency
    if (possibleAllocation.size() == 1)
    {
      cost = bid = 0;
    }
    else if (possibleAllocation.size() == 2)
    {
      path = Path::fromPair(
          std::make_pair(possibleAllocation.front(),
              possibleAllocation.back()));
      cost = path->getTotalCost(g->getGraph());
      bid = cost - rtc;

      delete path;
    }
    else
    {
      sub = g->getSubgraph(possibleAllocation);
      dp.dprintf(LVL_BID, "Subgraph:\n%s\n", sub->toString().c_str());

      tree = SpanningTree::fromGraph(sub->getGraph());
      dp.dprintf(LVL_BID, "Spanning Tree:\n%s\n", tree->toString().c_str());

      path = Path::fromTree(tree);
      dp.dprintf(LVL_BID, "Path:\n%s\n", path->toString().c_str());

      cost = path->getTotalCost(g->getGraph());
      bid = cost - rtc;

      delete path;
      delete tree;
      delete sub;
    }

    dp.dprintf(LVL_BID, "bid[%lu_%s]=%s (cost-rtc)=(%s-%s)\n", roundNumber,
        boost::lexical_cast<std::string>(*t).c_str(),
        boost::lexical_cast<std::string>(bid).c_str(),
        boost::lexical_cast<std::string>(cost).c_str(),
        boost::lexical_cast<std::string>(rtc).c_str()
    );

    if (currentBid.second > bid && bid >= 0)
      currentBid = std::make_pair(*t, bid);

    dp.dprintf(LVL_BID, "cur_bid=%s:%s\n",
        boost::lexical_cast<std::string>(currentBid.first).c_str(),
        boost::lexical_cast<std::string>(currentBid.second).c_str());
  }

  assert(currentBid.first  != MAX_VERTEX
      && currentBid.second != MAX_WEIGHT);

  // Send bid to MOOSDB
  doNotify(getBidVar(id), bidToString(currentBid));
}

void
Bidder::performFinalCalc(void)
{
  // Do final cost calculation and submit path
  Subgraph *sub = Subgraph::fromGraph(g, allocated);
  SpanningTree *tree = SpanningTree::fromGraph(sub->getGraph());
  Path *path = Path::fromTree(tree);
  Loc *locs = new Loc[path->getLength()];

  // Debug output
  DebugLevel LVL_FINAL_PATH = LVL_MID_VERB;
  if (dp.isValidLevel(LVL_FINAL_PATH)) // extra check to avoid extra work
  {
    dp.dprintf(LVL_FINAL_PATH, "Final allocated:\n");
    int count = 0;
    for (std::vector<Vertex>::iterator it = allocated.begin(); it != allocated.end(); it++)
    {
      dp.dprintf(LVL_FINAL_PATH, "\tallocated[%i]:%s\n", count++,
          boost::lexical_cast<std::string>(*it).c_str());
    }
    dp.dprintf(LVL_FINAL_PATH, "Final path:\n%s\n", path->toString().c_str());
  }

  // Convert the path indices to those in the origin graph
  path->convertPath(sub->getParentIndices());
  dp.dprintf(LVL_FINAL_PATH, "Converted path:\n%s\n", path->toString().c_str());

  // Get the coordinates of the vertices
  path->getLocations(__locations, locs);

  // Send the path to the MOOSDB
  doNotify(getPathVar(id), getPathVarVal(pathToString(locs, path->getLength())));

  delete sub;
  delete tree;
  delete path;
  delete locs;
}

bool
Bidder::OnStartUp(void)
{
  bool ret;

  // Read the DebugOutput configuration field
  int debugLevel;
  if (!m_MissionReader.GetConfigurationParam("DebugOutput", debugLevel))
    debugLevel = LVL_OFF;
  dp.setLevel((DebugLevel)debugLevel);

  ret = AuctionMOOSApp::OnStartUp();

  // Read the AgentID configuration field
  if (!m_MissionReader.GetConfigurationParam("AgentID", id))
  {
    MOOSTrace("Warning: parameter 'AgentID' not specified.\n");
    MOOSTrace("Terminating\n");
    exit(-1);
  }

  RegisterVariables();

  return ret;
}

bool
Bidder::OnConnectToServer(void)
{
  bool ret = AuctionMOOSApp::OnConnectToServer();
  RegisterVariables();
  return ret;
}

void
Bidder::RegisterVariables(void)
{
  m_Comms.Register(MVAR_BID_WINNER, 0);
  m_Comms.Register(MVAR_BID_START, 0);
  m_Comms.Register(MVAR_BID_TARGETS, 0);
}
