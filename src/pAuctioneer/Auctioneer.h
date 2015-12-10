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

#include <string>

class Auctioneer : public CMOOSApp
{
public:
  Auctioneer(void);
  ~Auctioneer(void);

  bool OnNewMail(MOOSMSG_LIST &NewMail);
  bool OnStartUp(void);
  bool Iterate(void);
  void RegisterVariables(void);
  bool OnConnectToServer(void);

protected:
  unsigned long int m_tally_recd;
  unsigned long int m_tally_sent;
  unsigned long int m_iterations;

private:
  size_t roundNumber;
  int numberOfBidders;
  Bid *bids;
  int numReceivedBids;
};

#endif /* MBOGO_AUCTIONEER_H_ */
