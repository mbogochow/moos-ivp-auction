

#ifndef AUCTION_DEFS_H_
#define AUCTION_DEFS_H_

#include <string>

#include "../lib_graphs/defs.h"

const std::string MVAR_BID_WINNER = "BID_WINNER";
const std::string MVAR_BID_START  = "BID_START";
const std::string MVAR_BID_HEADER = "BID_";

typedef struct WinningBid
{
  int winner;
  Vertex target;
  mbogo_weight_t bid;
} WinningBid;

typedef std::pair<Vertex, mbogo_weight_t> Bid;

std::string bidToString(Bid bid);
Bid bidFromString(std::string str);
std::string winningBidToString(WinningBid bid);
WinningBid winningBidFromString(std::string str);
int getBidder(std::string bidHeader);
std::string getBidVar(int id);

#endif /*AUCTION_DEFS_H_*/
