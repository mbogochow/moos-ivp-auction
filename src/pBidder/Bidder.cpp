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
#include "../lib_graphs/SpanningTree.h"
#include "../lib_graphs/Path.h"
#include "../lib_graphs/defs.h"
#include "../lib_auction/AuctionDefs.h"
#include "../lib_auction/DebugPrinter.h"

#include "MBUtils.h"

#include <algorithm> // std::remove
#include <boost/lexical_cast.hpp>

Bidder::Bidder(void)
{
  m_iterations = 0;

  g = new Graph(__edges, __num_edges, __weights, __num_nodes);

  // TODO add me to the graph

  allocated.reserve(__num_nodes);
  unallocated.reserve(__num_nodes);

  for (Vertex i = 0; i < __num_nodes; i++)
    unallocated.push_back(i);

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
  bool ret = true;

  dp.dprintf(LVL_MAX_VERB, "OnNewMail()\n");

  if (roundNumber < g->getNumVertices()) //TODO come up with better solution of stopping auctioneer
  {
    MOOSMSG_LIST::reverse_iterator p;
    for(p = NewMail.rbegin(); p!=NewMail.rend(); p++) {
      CMOOSMsg &msg = *p;
      std::string key   = msg.GetKey();

      if (key == MVAR_BID_START)
      {
        roundUpdated = true;
        roundNumber += 1;
        dp.dprintf(LVL_MIN_VERB, "Got %s mail: %s\n", MVAR_BID_START.c_str(),
            msg.GetString().c_str());
      }
      else if (key == MVAR_BID_WINNER)
      {
        winnerUpdated = true;
        winningBid = winningBidFromString(msg.GetString());
        dp.dprintf(LVL_MIN_VERB, "Got %s mail: %s\n", MVAR_BID_WINNER.c_str(),
            winningBid);
      }
    }
  }

  return ret;
}

bool
Bidder::Iterate(void)
{
  bool ret = true;

  m_iterations += 1;
  dp.dprintf(LVL_MID_VERB, "Iterate() #%u\n", m_iterations);

  if (roundNumber < g->getNumVertices()) //TODO come up with better solution of stopping auctioneer
  {
    if (winnerUpdated)
    {
      if (winningBid.winner == id)
      {
        allocated.push_back(winningBid.target);
        unallocated.erase(
            std::remove(unallocated.begin(), unallocated.end(),
                winningBid.target), unallocated.end());
        rtc += winningBid.bid;
      }
    }

    if (roundUpdated)
    {
      performBiddingRound();
    }
  }
  else
  {
    // TODO Write waypoints from path for agent to follow then exit process
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
//std::cerr << "Adding " << *t << " for possible allocation" << std::endl;
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
//sub->print();

      tree = SpanningTree::fromGraph(sub->getGraph());
//std::cout << "Spanning Tree:" << std::endl;
//tree->print();

      path = Path::fromTree(tree);

      cost = path->getTotalCost(g->getGraph());//= get_path_cost(g, path); // TODO
      bid = cost - rtc;

      delete path;
      delete tree;
      delete sub;
    }

//std::cerr << "bid[" << i << "_" << *t << "]=" << bid << " (cost-rtc)=(" << cost << "-" << rtc << ")" << std::endl;

    if (currentBid.second > bid && bid >= 0)
      currentBid = std::make_pair(*t, bid);

//std::cerr << "cur_bid=" << currentBid.first << ":" << currentBid.second << std::endl;
  }

  assert(currentBid.first  != MAX_VERTEX
      && currentBid.second != MAX_WEIGHT);

  // Send bid
//  f_Comms->writeMsg(MVAR_BID_HEADER + intToString(id), bidToString(currentBid));
  if (!Notify(getBidVar(id), bidToString(currentBid)))
  {
    MOOSTrace("ERROR: Failed to write %s=%s to MOOSDB\n", getBidVar(id).c_str(),
        bidToString(currentBid).c_str());
  }
}

bool
Bidder::OnStartUp(void)
{
  bool ret = true;

  // Read the DebugOutput configuration field
  int debugLevel;
  if (!m_MissionReader.GetConfigurationParam("DebugOutput", debugLevel))
    debugLevel = LVL_OFF;
  dp.setLevel((DebugLevel)debugLevel);
  dp.dprintf(LVL_MAX_VERB, "OnStartup()\n");

  // Read the AgentID configuration field
  std::string id;
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
  dp.dprintf(LVL_MAX_VERB, "OnConnectToServer()\n");
  RegisterVariables();
  return true;
}

void
Bidder::RegisterVariables(void)
{
  m_Comms.Register("BID_WINNER", 0);
  m_Comms.Register("BID_START", 0);
}
