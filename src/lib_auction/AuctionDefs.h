

#ifndef AUCTION_DEFS_H_
#define AUCTION_DEFS_H_

#include <string>

#include "../lib_graphs/defs.h"

#include "DebugPrinter.h"

const std::string MVAR_BID_WINNER  = "BID_WINNER";
const std::string MVAR_BID_START   = "BID_START";
const std::string MVAR_BID_TARGETS = "BID_TARGETS";
const std::string MVAR_BID_HEADER  = "BID_V";
const std::string MVAR_PATH_HEADER = "PATH_UPDATE_V";
const std::string MVAR_PATH        = "points";

typedef struct WinningBid
{
  int winner;
  Vertex target;
  mbogo_weight_t bid;
} WinningBid;

typedef std::pair<Vertex, mbogo_weight_t> Bid;

template<typename U, typename V>
std::string pairToString(std::pair<U,V> pair);

std::string bidToString(Bid bid);
Bid bidFromString(std::string str);
std::string winningBidToString(WinningBid bid);
WinningBid winningBidFromString(std::string str);
int getBidder(std::string bidHeader);
std::string getVar(std::string header, int id);
std::string getBidVar(int id);
std::string getPathVar(int id);
std::string getPathVarVal(std::string sPath);

std::string pathToString(Loc locs[], size_t numLocs);
void pathFromString(std::string str, std::vector<Loc> path);
size_t getStringPathSize(std::string sPath);

double getDistance(Loc loc1, Loc loc2);

int nthTriangularNumber(int n);

void connectEdges(std::vector<Loc> targets, std::vector<Edge> edges,
    std::vector<mbogo_weight_t> weights);

#endif /*AUCTION_DEFS_H_*/
