

#include "../lib_graphs/defs.h"

#include "DebugPrinter.h"

#include "AuctionDefs.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <cmath>

const size_t __num_nodes = 9;
const size_t __num_edges = 36;
Edge __edges[__num_edges] = {
  Edge(_a, _b), Edge(_a, _c), Edge(_a, _d), Edge(_a, _e), Edge(_a, _f), Edge(_a, _g), Edge(_a, _h), Edge(_a, _i),
  Edge(_b, _c), Edge(_b, _d), Edge(_b, _e), Edge(_b, _f), Edge(_b, _g), Edge(_b, _h), Edge(_b, _i),
  Edge(_c, _d), Edge(_c, _e), Edge(_c, _f), Edge(_c, _g), Edge(_c, _h), Edge(_c, _i),
  Edge(_d, _e), Edge(_d, _f), Edge(_d, _g), Edge(_d, _h), Edge(_d, _i),
  Edge(_e, _f), Edge(_e, _g), Edge(_e, _h), Edge(_e, _i),
  Edge(_f, _g), Edge(_f, _h), Edge(_f, _i),
  Edge(_g, _h), Edge(_g, _i),
  Edge(_h, _i)
};
int __weights[__num_edges] = {
    4,       8,       11,      8,      7,        4,       2,      9,
    14,      10,      2,       1,      6,        7,       4,
    8,       11,      8,       7,      4,        2,
    9,       14,      10,      2,      1,
    6,       7,       4,       8,
    11,      8,       7,
    4,       2,
    9 };
//Loc __locations[__num_nodes] = {
//    Loc(25, 0), Loc(50, 50), Loc(100, 50), Loc(150, 50),
//    Loc(200, 0), Loc(150, -50), Loc(100, -50), Loc(50, -50), Loc(75, 0)
//};

Loc __locations[__num_nodes] = {
    Loc(7, 0), Loc(15, 15), Loc(30, 15), Loc(45, 15),
    Loc(60, 0), Loc(45, -15), Loc(30, -15), Loc(15, -15), Loc(22, 0)
};
//Loc __locations[__num_nodes] = {
//    Loc(0.5, 0), Loc(1, 1), Loc(2, 1), Loc(3, 1),
//    Loc(4, 0), Loc(3, -1), Loc(2, -1), Loc(1, -1), Loc(1.5, 0)
//};

std::string pathToString(Loc path[], const size_t pathSize)
{
  std::stringstream ss;
  size_t i;

  // Cycle through all but last one
  for (i = 0; i < pathSize - 1; i++)
    ss << path[i].first << "," << path[i].second << ":";

  // Finish with last one
  ss << path[i].first << "," << path[i].second;

  return ss.str();
}

template<typename U, typename V>
std::string pairToString(std::pair<U,V> pair)
{
  std::stringstream ss;
  ss << pair.first << "," << pair.second;
  return ss.str();
}

std::string bidToString(Bid bid)
{
  return pairToString(bid);
}

Bid bidFromString(std::string str)
{
  Bid bid;
  std::vector<std::string> strs;
  boost::split(strs, str, boost::is_any_of(","));
  bid = std::make_pair(
      boost::lexical_cast<Vertex>(strs[0]),
      boost::lexical_cast<mbogo_weight_t>(strs[1]));
  return bid;
}

std::string winningBidToString(WinningBid bid)
{
  std::stringstream ss;
  ss << bid.winner << "," << bid.target << "," << bid.bid;
  return ss.str();
}

WinningBid winningBidFromString(std::string str)
{
  WinningBid bid;
  std::vector<std::string> strs;
  boost::split(strs, str, boost::is_any_of(","));
  bid.winner = boost::lexical_cast<int>(strs[0]);
  bid.target = boost::lexical_cast<Vertex>(strs[1]);
  bid.bid    = boost::lexical_cast<mbogo_weight_t>(strs[2]);
  return bid;
}

int getBidder(std::string bidHeader)
{
  int bidder;

  std::vector<std::string> strs;
  boost::split(strs, bidHeader, boost::is_any_of("_V"));
  bidder = boost::lexical_cast<int>(strs.back());

  return bidder;
}

std::string getVar(std::string header, const int id)
{
  std::stringstream ss;
  ss << header << id;
  return ss.str();
}

std::string getBidVar(const int id)
{
  return getVar(MVAR_BID_HEADER, id);
}

std::string getPathVar(int id)
{
  return getVar(MVAR_PATH_HEADER, id);
}

std::string getPathVarVal(std::string sPath)
{
  std::stringstream ss;
  ss << MVAR_PATH << "=" << sPath;
  return ss.str();
}

void pathFromString(std::string str, std::vector<Loc> path)
{
  std::vector<std::string> locs;
  boost::split(locs, str, boost::is_any_of(":"));
  path.reserve(locs.size());
  for (std::vector<std::string>::iterator it = locs.begin(); it != locs.end();
      it++)
  {
    std::vector<std::string> points;
    boost::split(points, *it, boost::is_any_of(","));
    assert(points.size() == 2);
    path.push_back(std::make_pair(
        boost::lexical_cast<double>(points.front()),
        boost::lexical_cast<double>(points.back())));
  }
}

size_t getStringPathSize(std::string sPath)
{
  std::vector<std::string> locs;
  boost::split(locs, sPath, boost::is_any_of(":"));
  return locs.size();
}

double getDistance(const Loc loc1, const Loc loc2)
{
  return std::sqrt(
      std::pow(loc2.first - loc1.first, 2) -
      std::pow(loc2.second - loc1.second, 2));
}
