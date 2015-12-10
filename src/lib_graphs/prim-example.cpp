/**
 * @author Mike Bogochow
 * @version 2.5.0, Dec 8, 2015
 *
 * @file main.cpp
 *
 * pBidder main
 */

#include "defs.h"
#include "Path.h"
#include "SpanningTree.h"
#include "Graph.h"

// boost
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
//#include <boost/lexical_cast.hpp>
#include <boost/graph/graph_utility.hpp>

#include <iostream>
#include <algorithm> // std::remove
#include <limits>

//#define _a 0
//#define _b 1
//#define _c 2
//#define _d 3
//#define _e 4
//#define _f 5
//#define _g 6
//#define _h 7
//#define _i 8

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

/**
 * Main
 */
int main(int argc, char **argv)
{
  Graph *g = new Graph(__edges, __num_edges, __weights, __num_nodes);
//g->print();
  // TODO add my location to graph

  Graph *sub;
  std::vector<Vertex> allocated;
  std::vector<Vertex> unallocated;
  unallocated.reserve(__num_nodes);

  int rtc = 0; // my cost
  for (Vertex i = 0; i < __num_nodes; i++)
    unallocated.push_back(i);

  // each round of bidding
  for (Vertex i = 0; i < __num_nodes; i++)
  {
    std::pair<Vertex, mbogo_weight_t> currentBid = std::make_pair(
        std::numeric_limits<Vertex>::max(),
        std::numeric_limits<mbogo_weight_t>::max());

//std::cerr << "~~~ Round " << i+1 << std::endl;
    // each unallocated target
    for (std::vector<Vertex>::iterator t = unallocated.begin();
        t != unallocated.end(); t++)
    {
      Path *path;
      SpanningTree *tree;
      mbogo_weight_t cost;
      mbogo_weight_t bid;
      std::vector<Vertex> possibleAllocation;

      possibleAllocation = allocated;
//std::cerr << "Adding " << *t << " for possible allocation" << std::endl;
      possibleAllocation.push_back(*t);

      // Catch simple cases for efficiency
      if (possibleAllocation.size() == 1)
      {
        cost = bid = 0;
      }
      else if (possibleAllocation.size() == 2)
      {
        path = Path::fromPair(
            std::make_pair(possibleAllocation.front(),
                possibleAllocation.back()));
        cost = path->getTotalCost(g->getGraph());
        bid = cost - rtc;

        delete path;
      }
      else
      {
        sub = g->getSubgraph(possibleAllocation);
//sub->print();

        tree = SpanningTree::fromGraph(sub->getGraph());
//std::cout << "Spanning Tree:" << std::endl;
//tree->print();

        path = Path::fromTree(tree);

        cost = path->getTotalCost(g->getGraph());//= get_path_cost(g, path); // TODO
        bid = cost - rtc;

        delete path;
        delete tree;
        delete sub;
      }

//std::cerr << "bid[" << i << "_" << *t << "]=" << bid << " (cost-rtc)=(" << cost << "-" << rtc << ")" << std::endl;

      if (currentBid.second > bid && bid >= 0)
        currentBid = std::make_pair(*t, bid);

//std::cerr << "cur_bid=" << currentBid.first << ":" << currentBid.second << std::endl;
    }

    // submit current bid
    // get results

    // if win
    //   allocated.push_back(winning target);
    //   unallocated.erase(std::remove(vec.begin(), vec.end(), winning target), vec.end());
    //   update rtc

    // TODO change
    int winner = unallocated.front();
    allocated.push_back(winner);
    unallocated.erase(std::remove(unallocated.begin(), unallocated.end(), winner), unallocated.end());
    rtc += currentBid.second;
  }

  // Do final calculation with allocated nodes
  sub = g->getSubgraph(allocated);
  SpanningTree *tree = SpanningTree::fromGraph(sub->getGraph());

  // Output spanning tree
  std::cout << "Spanning Tree:" << std::endl;
  tree->print();

  // Get the path from the tree
  Path *path = Path::fromTree(tree);
  mbogo_weight_t pathCost = path->getTotalCost(g->getGraph());//get_path_cost(g, path);

  // Output the path
  std::cout << "Path:" << std::endl;
  path->print();
  std::cerr << "cost: " << pathCost << std::endl;

  delete path;
  delete tree;
  delete g;
  delete sub;

  return EXIT_SUCCESS;
}
