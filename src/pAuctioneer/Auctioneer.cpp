/**
 * @author Mike Bogochow
 * @version 2.4.0, Dec 8, 2015
 *
 * @file Auctioneer.cpp
 *
 * Auctioneer class implementation
 */

#include "Auctioneer.h"
#include "../lib_auction/AuctionDefs.h"
#include "../lib_auction/DebugPrinter.h"

#include "MBUtils.h"

#include <boost/algorithm/string/predicate.hpp> // starts_with
#include <chrono>
#include <thread>

Auctioneer::Auctioneer(void)
{
  numBidders = 0;
  numTargets = 0;

  roundNumber = 0;
  numRecvdBids = 0;
  bids = nullptr;
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

  MOOSMSG_LIST::reverse_iterator p;
  for(p = NewMail.rbegin(); p != NewMail.rend(); p++)
  {
    CMOOSMsg &msg = *p;
    std::string key   = msg.GetKey();

    if (boost::starts_with(key, MVAR_BID_HEADER))
    {
      int bidder = getBidder(key);
      bids[bidder] = bidFromString(msg.GetString());
      numRecvdBids += 1;
      dp.dprintf(LVL_MIN_VERB, "Got %s mail: %s\n", key.c_str(),
          msg.GetString().c_str());
    }
  }

  return ret;
}

bool
Auctioneer::Iterate(void)
{
  bool ret = AuctionMOOSApp::Iterate();

  dp.dprintf(LVL_MAX_VERB, "roundNum <= numNodes (%lu <= %lu)?\n", roundNumber, numTargets);
  if (roundNumber <= numTargets)
  {
    dp.dprintf(LVL_MID_VERB, "numReceivedBids=%i\n", numRecvdBids);

    if (roundNumber == 0)
      doNotify(MVAR_BID_START, ++roundNumber);
    else if (numRecvdBids == numBidders)
    { // All bids received for round
      WinningBid winner = { 0, MAX_VERTEX, MAX_WEIGHT };
      // Calculate winner
      for (size_t i = 0; i < numBidders; i++)
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

      numRecvdBids = 0;
    }
    else
    { // Keep sending the round number in case they didn't get the message.
      doNotify(MVAR_BID_START, roundNumber);
      if (roundNumber == 1)
        doNotify(MVAR_BID_TARGETS, targets);
    }
  }
  else
  {
    // Exit pAuctioneer
    doNotify("EXITED_NORMALLY", "pAuctioneer");
    // Give the message some time to get there before closing
    std::this_thread::sleep_for(std::chrono::seconds(10));
    exit(0);
  }

  return ret;
}

bool
Auctioneer::OnStartUp(void)
{
  bool ret;

  // DebugOutput config
  int debugLevel;
  if (!m_MissionReader.GetConfigurationParam("DebugOutput", debugLevel))
    debugLevel = LVL_OFF;
  dp.setLevel((DebugLevel)debugLevel);

  ret = AuctionMOOSApp::OnStartUp();

  if (ret)
  {
    // NumBidders config
    std::string tmp;
    if (!m_MissionReader.GetConfigurationParam("NumBidders", tmp))
      ret = MissingRequiredParam("NumBidders");
    else
    {
      numBidders = boost::lexical_cast<size_t>(tmp);
      this->bids = new Bid[numBidders];
    }

    if (ret)
    {
      // Targets config
      if (!m_MissionReader.GetConfigurationParam("Targets", targets))
        ret = MissingRequiredParam("Targets");
      else
      {
        numTargets = getStringPathSize(targets);
        doNotify(MVAR_BID_TARGETS, targets);
      }

      if (ret)
        RegisterVariables();
    }
  }

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
  for (size_t i = 0; i < numBidders; i++)
  {
    std::string key = getBidVar(i);
    dp.dprintf(LVL_MIN_VERB, "Registering %s\n", key.c_str());
    m_Comms.Register(key, 0);
  }
}
