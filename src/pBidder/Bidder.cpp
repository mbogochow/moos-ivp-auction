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
  if (g != nullptr)
    delete g;
}

//#define _a 0
//#define _b 1
//#define _c 2
//#define _d 3
//#define _e 4
//#define _f 5
//#define _g 6
//#define _h 7
//#define _i 8
//
//const size_t __num_nodes = 9;
//const size_t __num_edges = 36;
//Edge __edges[__num_edges] = {
//  Edge(_a, _b), Edge(_a, _c), Edge(_a, _d), Edge(_a, _e), Edge(_a, _f), Edge(_a, _g), Edge(_a, _h), Edge(_a, _i),
//  Edge(_b, _c), Edge(_b, _d), Edge(_b, _e), Edge(_b, _f), Edge(_b, _g), Edge(_b, _h), Edge(_b, _i),
//  Edge(_c, _d), Edge(_c, _e), Edge(_c, _f), Edge(_c, _g), Edge(_c, _h), Edge(_c, _i),
//  Edge(_d, _e), Edge(_d, _f), Edge(_d, _g), Edge(_d, _h), Edge(_d, _i),
//  Edge(_e, _f), Edge(_e, _g), Edge(_e, _h), Edge(_e, _i),
//  Edge(_f, _g), Edge(_f, _h), Edge(_f, _i),
//  Edge(_g, _h), Edge(_g, _i),
//  Edge(_h, _i)
//};
//int __weights[__num_edges] = {
//    4,       8,       11,      8,      7,        4,       2,      9,
//    14,      10,      2,       1,      6,        7,       4,
//    8,       11,      8,       7,      4,        2,
//    9,       14,      10,      2,      1,
//    6,       7,       4,       8,
//    11,      8,       7,
//    4,       2,
//    9 };
//Point __Pointations[__num_nodes] = {
//    Point(7, 0), Point(15, 15), Point(30, 15), Point(45, 15),
//    Point(60, 0), Point(45, -15), Point(30, -15), Point(15, -15), Point(22, 0)
//};

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
MOOSTrace("sTargets: %s\n", sTargets.c_str());
//std::this_thread::sleep_for(std::chrono::seconds(4));
      pathFromString(sTargets, targets);
MOOSTrace("HERE\n");
//std::this_thread::sleep_for(std::chrono::seconds(4));
      size_t numTargets = targets.size();
MOOSTrace("targets.size: %s\n", numTargets);
//std::this_thread::sleep_for(std::chrono::seconds(4));
      std::vector<Edge> edges;
      std::vector<mbogo_weight_t> weights;
      connectEdges(targets, edges, weights);
MOOSTrace("edges.size: %s\n", edges.size());
//std::this_thread::sleep_for(std::chrono::seconds(4));
MOOSTrace("weights.size: %s\n", weights.size());
//std::this_thread::sleep_for(std::chrono::seconds(4));

      g = new Graph(edges.data(), edges.size(), weights.data(), numTargets);
//      g = new Graph(__edges, __num_edges, __weights, __num_nodes);

      // TODO add me to the graph

//      size_t numTargets = __num_nodes;

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
    { // Update info with new winner
      if (winningBid.winner == id)
      { // I won so add winning target to my allocated targets
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
      { // Do round calculations for new round
        performBiddingRound();
        roundUpdated = false;
      }
    }
    else
    { // All rounds complete so perform final calculation and post
      assert(unallocated.size() == 0);

      performFinalCalc();

      // Exit pBidder
      doNotify("EXITED_NORMALLY", "pBidder");
      // Ensure all data makes it to the MOOSDB
      std::this_thread::sleep_for(std::chrono::seconds(10));
      // OR
        // SetQuitOnFailedIterate(true);
        // ret = false;
      // OR
        // RequestQuit();
      exit(0);
    }
  }
  else
    dp.dprintf(LVL_MAX_VERB, "Graph not initialized...\n");

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
  Point *pathPoints = new Point[path->getLength()];

  // Debug output
  DebugLevel LVL_FINAL_PATH = LVL_MID_VERB;
  if (dp.isValidLevel(LVL_FINAL_PATH)) // extra check to avoid extra work
  {
    dp.dprintf(LVL_FINAL_PATH, "Final allocated:\n");
    int count = 0;
    for (std::vector<Vertex>::iterator it = allocated.begin();
        it != allocated.end(); it++)
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
  path->getLocations(targets.data(), pathPoints);

  // Send the path to the MOOSDB
  doNotify(getPathVar(id),
      getPathVarVal(pathToString(pathPoints, path->getLength())));

  delete sub;
  delete tree;
  delete path;
  delete pathPoints;
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
  if (ret && !m_MissionReader.GetConfigurationParam("AgentID", id))
    ret = MissingRequiredParam("AgentID");

  if (ret)
  {
    std::string tmp;
    if (!m_MissionReader.GetConfigurationParam("START_POS", tmp))
      ret = MissingRequiredParam("START_POS");
    else
      startPos = pointFromString(tmp);

    if (ret)
      RegisterVariables();
  }

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
