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
#include "../lib_auction/AuctionMOOSApp.h"

#include <boost/lexical_cast.hpp>

#include <string>

/**
 * A bidder in an auction.
 */
class Bidder : public AuctionMOOSApp
{
public:
  Bidder(void);
  ~Bidder(void);

  bool OnNewMail(MOOSMSG_LIST &NewMail);
  bool OnStartUp(void);
  bool Iterate(void);
  void RegisterVariables(void);
  bool OnConnectToServer(void);

private:
  // Auction state info
  /** The graph of targets */
  Graph *g;
  /** The list of target locations */
  std::vector<Loc> targets;
  /** The current list of unallocated targets */
  std::vector<Vertex> unallocated;

  // Bidder info
  /** Bidder's list of allocated targets */
  std::vector<Vertex> allocated;
  /** Bidder's current robot tree cost for my allocated nodes */
  mbogo_weight_t rtc;
  /** The Bidder's ID */
  int id;

  // Auction round info
  /** Current round number */
  size_t roundNumber;
  /** Whether the round number has been updated since last calculation */
  bool roundUpdated;
  /** Whether the winner has been updated since last calculation */
  bool winnerUpdated;
  /** The winning bid from the last round */
  struct WinningBid winningBid;

  /** Debug level for bid calculation output */
  static const DebugLevel LVL_BID = LVL_MAX_VERB;

  /**
   * Perform all calculations necessary for a bidding round and post the
   * round's bid to the MOOSDB.
   */
  void performBiddingRound(void);
  /**
   * Perform the final path calculation after all bidding rounds have completed
   * and post the final path to the MOOSDB.
   */
  void performFinalCalc(void);
};

#endif /* MBOGO_BIDDER_H_ */
