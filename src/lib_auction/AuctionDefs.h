

#ifndef AUCTION_DEFS_H_
#define AUCTION_DEFS_H_

#include "../lib_graphs/defs.h"

#include "DebugPrinter.h"

#include <string>

const std::string MVAR_BID_WINNER  = "BID_WINNER";
const std::string MVAR_BID_START   = "BID_START";
const std::string MVAR_BID_TARGETS = "BID_TARGETS";
const std::string MVAR_BID_HEADER  = "BID_V";
const std::string MVAR_PATH_HEADER = "PATH_UPDATE_V";
const std::string MVAR_PATH        = "points";
const std::string MVAR_COST        = "FINAL_COST_V";
const std::string MVAR_TIME        = "TOTAL_TIME_V";

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

std::string pathToString(Point path[], size_t pathSize);
void pathFromString(std::string str, std::vector<Point> &path);
Point pointFromString(std::string str);
size_t getStringPathSize(std::string sPath);

double getDistance(Point p1, Point p2);

int nthTriangularNumber(int n);

void connectEdges(std::vector<Point> targets, std::vector<Edge> &edges,
    std::vector<mbogo_weight_t> &weights);

void fixTargets(std::vector<Point> targets);

#endif /*AUCTION_DEFS_H_*/
