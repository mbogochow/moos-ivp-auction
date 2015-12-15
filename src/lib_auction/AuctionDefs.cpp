

#include "../lib_graphs/defs.h"

#include "DebugPrinter.h"

#include "AuctionDefs.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

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
Loc __locations[__num_nodes] = {
    Loc(0.5, 0), Loc(1, 1), Loc(2, 1), Loc(3, 1),
    Loc(4, 0), Loc(3, -1), Loc(2, -1), Loc(1, -1), Loc(1.5, 0)
};

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

  int count = 0;
  for (std::vector<std::string>::iterator it = strs.begin(); it != strs.end(); it++)
  {
    std::cerr << "split[" << count++ << "]=" << *it << std::endl;
  }
  std::cerr << "split.back()=" << strs.back() << std::endl;

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

