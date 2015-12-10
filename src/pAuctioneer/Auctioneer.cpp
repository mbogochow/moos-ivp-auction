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

#include "MBUtils.h"

#include <boost/algorithm/string/predicate.hpp> // starts_with

Auctioneer::Auctioneer(void)
{
  m_tally_recd = 0;
  m_tally_sent = 0;
  m_iterations = 0;

  roundNumber = -1;
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
  bool ret = true;

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
  bool ret = true;

  if (roundNumber < __num_nodes) //TODO come up with better solution of stopping auctioneer
  {
    if (roundNumber == 0)
      m_Comms.Notify(MVAR_BID_START, ++roundNumber);
    else if (numReceivedBids == numberOfBidders)
    {
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
      m_Comms.Notify(MVAR_BID_WINNER, winningBidToString(winner));

      m_Comms.Notify(MVAR_BID_START, ++roundNumber);

      numReceivedBids = 0;
    }
  }

  return ret;
}

bool
Auctioneer::OnStartUp(void)
{
  bool ret = true;

  STRING_LIST sParams;
  m_MissionReader.GetConfiguration(GetAppName(), sParams);

  STRING_LIST::iterator p;
  for(p = sParams.begin(); p != sParams.end(); p++)
  {
//      string sLine     = *p;
//      string sVarName  = MOOSChomp(sLine, "=");
//      sLine = stripBlankEnds(sLine);
//
//      if(MOOSStrCmp(sVarName, "INCOMING_VAR")) {
//        if(!strContains(sLine, " "))
//    m_incoming_var = stripBlankEnds(sLine);
//      }
//
//      else if(MOOSStrCmp(sVarName, "OUTGOING_VAR")) {
//        if(!strContains(sLine, " "))
//    m_outgoing_var = stripBlankEnds(sLine);
//      }
    // TODO set number of bidders
  }

  return ret;
}

bool
Auctioneer::OnConnectToServer(void)
{
  RegisterVariables();
  return true;
}

void
Auctioneer::RegisterVariables(void)
{
  for (int i = 0; i < numberOfBidders; i++)
    m_Comms.Register(MVAR_BID_HEADER + intToString(i + 1), 0);
}
