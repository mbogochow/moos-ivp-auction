

#include "../lib_graphs/defs.h"

#include "DebugPrinter.h"

#include "AuctionDefs.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <cmath>

std::string pathToString(Point path[], const size_t pathSize)
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

Point pointFromString(std::string str)
{
  std::vector<std::string> points;
  boost::split(points, str, boost::is_any_of(","));
  assert(points.size() == 2);
  return std::make_pair(
      boost::lexical_cast<double>(points.front()),
      boost::lexical_cast<double>(points.back()));
}

void pathFromString(std::string str, std::vector<Point> path)
{
  std::vector<std::string> locs;
  boost::split(locs, str, boost::is_any_of(":"));
  path.reserve(locs.size());
  for (std::vector<std::string>::iterator it = locs.begin(); it != locs.end();
      it++)
  {
    path.push_back(pointFromString(*it));
  }
}

size_t getStringPathSize(std::string sPath)
{
  std::vector<std::string> locs;
  boost::split(locs, sPath, boost::is_any_of(":"));
  return locs.size();
}

double getDistance(const Point p1, const Point p2)
{
  return std::sqrt(
      std::pow(p2.first - p1.first, 2) - std::pow(p2.second - p1.second, 2));
}

int nthTriangularNumber(const int n)
{
  int sum = 0;
  for (int i = 1; i <= n; i++)
    sum += i;
  return sum;
}

void connectEdges(std::vector<Point> targets, std::vector<Edge> edges,
    std::vector<mbogo_weight_t> weights)
{
  size_t numTargets = targets.size();
  int numEdges = nthTriangularNumber(targets.size());
  Point *targs = targets.data();

  edges.reserve(numEdges);
  weights.reserve(numEdges);

  for (size_t i = 0; i < numTargets; i++)
  {
    for (size_t j = i + 1; j < numTargets; j++)
    {
      edges.push_back(std::make_pair(i, j));
      weights.push_back(getDistance(targs[i], targs[j]));
    }
  }
}
