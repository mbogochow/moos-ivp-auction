/**
 * @author Mike Bogochow
 * @version 2.4.0, Dec 8, 2015
 *
 * @file Auctioneer.h
 *
 * Auctioneer class
 */

#ifndef MBOGO_AUCTIONEER_H_
#define MBOGO_AUCTIONEER_H_

#include "MOOS/libMOOS/App/MOOSApp.h"

#include "../lib_auction/AuctionDefs.h"
#include "../lib_auction/AuctionMOOSApp.h"

#include <boost/lexical_cast.hpp>

#include <string>

class Auctioneer : public AuctionMOOSApp
{
public:
  Auctioneer(void);
  ~Auctioneer(void);

  bool OnNewMail(MOOSMSG_LIST &NewMail);
  bool OnStartUp(void);
  bool Iterate(void);
  void RegisterVariables(void);
  bool OnConnectToServer(void);

private:
  // Absolute values
  /** Number of bidders in the auction */
  size_t numBidders;
  /** Number of targets up for auction */
  size_t numTargets;

  // Per round values
  /** The current round number; controlled by auctioneer */
  size_t roundNumber;
  /** Number of bids received for current round */
  size_t numRecvdBids;
  /** The bids received during the current round */
  Bid *bids;
};

#endif /* MBOGO_AUCTIONEER_H_ */
