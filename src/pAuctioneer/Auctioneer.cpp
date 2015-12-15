/**
 * @author Mike Bogochow
 * @version 2.4.0, Dec 8, 2015
 *
 * @file Auctioneer.cpp
 *
 * Auctioneer class implementation
 */

#include "Auctioneer.h"
#include "../lib_graphs/defs.h"
#include "../lib_auction/AuctionDefs.h"
#include "../lib_auction/DebugPrinter.h"

#include "MBUtils.h"

#include <boost/algorithm/string/predicate.hpp> // starts_with
#include <boost/lexical_cast.hpp>

Auctioneer::Auctioneer(void)
{
  m_iterations = 0;

  roundNumber = 0;
  numberOfBidders = -1;
  bids = nullptr;
  numReceivedBids = 0;
}

Auctioneer::~Auctioneer(void)
{
  if (bids != nullptr)
    delete bids;
}

bool
Auctioneer::OnNewMail(MOOSMSG_LIST &NewMail)
{
  bool ret = AuctionMOOSApp::OnNewMail(NewMail);

  dp.dprintf(LVL_MID_VERB, "roundNum < numNodes (%lu < %lu)?\n", roundNumber, __num_nodes);
  if (roundNumber < __num_nodes) //TODO come up with better solution of stopping auctioneer
  {
    MOOSMSG_LIST::reverse_iterator p;
    for(p = NewMail.rbegin(); p!=NewMail.rend(); p++)
    {
      CMOOSMsg &msg = *p;
      std::string key   = msg.GetKey();

      if (boost::starts_with(key, MVAR_BID_HEADER))
      {
        int bidder = getBidder(key);
        bids[bidder] = bidFromString(msg.GetString());
        numReceivedBids += 1;
      }
    }
  }

  return ret;
}

bool
Auctioneer::Iterate(void)
{
  bool ret = AuctionMOOSApp::Iterate();

  dp.dprintf(LVL_MID_VERB, "roundNum < numNodes (%lu < %lu)?\n", roundNumber, __num_nodes);
  if (roundNumber < __num_nodes) //TODO come up with better solution of stopping auctioneer
  {
    if (roundNumber == 0)
      doNotify(MVAR_BID_START, ++roundNumber);
    else if (numReceivedBids == numberOfBidders)
    { // All bids received for round
      WinningBid winner = { 0, MAX_VERTEX, MAX_WEIGHT };
      // Calculate winner
      for (int i = 0; i < numberOfBidders; i++)
      {
        if (bids[i].second < winner.bid)
        {
          winner.winner = i;
          winner.target = bids[i].first;
          winner.bid = bids[i].second;
        }
      }

      // Send winner
      doNotify(MVAR_BID_WINNER, winningBidToString(winner));
      doNotify(MVAR_BID_START, ++roundNumber);

      numReceivedBids = 0;
    }
  }

  return ret;
}

bool
Auctioneer::OnStartUp(void)
{
  bool ret;

  // Read the DebugOutput configuration field
  int debugLevel;
  if (!m_MissionReader.GetConfigurationParam("DebugOutput", debugLevel))
    debugLevel = LVL_OFF;
  dp.setLevel((DebugLevel)debugLevel);

  ret = AuctionMOOSApp::OnStartUp();

  if (!m_MissionReader.GetConfigurationParam("NumBidders", numberOfBidders))
  {
    MOOSTrace("Warning: parameter 'NumBidders' not specified.\n");
    MOOSTrace("Terminating\n");
    exit(-1);
  }
  else
    this->bids = new Bid[numberOfBidders];

  return ret;
}

bool
Auctioneer::OnConnectToServer(void)
{
  bool ret = AuctionMOOSApp::OnConnectToServer();
  RegisterVariables();
  return ret;
}

void
Auctioneer::RegisterVariables(void)
{
  for (int i = 0; i < numberOfBidders; i++)
    m_Comms.Register(getBidVar(i), 0);
}
