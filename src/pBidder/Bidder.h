/**
 * @author Mike Bogochow
 * @version 2.4.0, Dec 8, 2015
 *
 * @file Bidder.h
 *
 * Bidder class
 */

#ifndef MBOGO_BIDDER_H_
#define MBOGO_BIDDER_H_

#include "MOOS/libMOOS/App/MOOSApp.h"

#include "../lib_graphs/Graph.h"
#include "../lib_graphs/defs.h"

#include "../lib_auction/AuctionDefs.h"

#include <string>

class Bidder : public CMOOSApp
{
public:
  Bidder(void);
  ~Bidder(void);

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
  Graph *g;
  std::vector<Vertex> allocated;
  std::vector<Vertex> unallocated;
  mbogo_weight_t rtc;
  int id;

  size_t roundNumber;
  bool roundUpdated;
  bool winnerUpdated;
  struct WinningBid winningBid;

  void performBiddingRound(void);
};

#endif /* MBOGO_BIDDER_H_ */
