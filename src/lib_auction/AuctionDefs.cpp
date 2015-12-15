

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

std::string bidToString(Bid bid)
{ // TODO reimplement for efficiency
  std::string str;
  str.append(boost::lexical_cast<std::string>(bid.first));
  str.append(",");
  str.append(boost::lexical_cast<std::string>(bid.second));
  return str;
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
{ // TODO reimplement for efficiency
  std::string str;
  str.append(boost::lexical_cast<std::string>(bid.winner));
  str.append(",");
  str.append(boost::lexical_cast<std::string>(bid.target));
  str.append(",");
  str.append(boost::lexical_cast<std::string>(bid.bid));
  return str;
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
  boost::split(strs, bidHeader, boost::is_any_of("_"));
  bidder = boost::lexical_cast<int>(strs.back());

  return bidder;
}

std::string getBidVar(const int id)
{
  std::stringstream ss;

  ss << MVAR_BID_HEADER << id;

  return ss.str();
}
