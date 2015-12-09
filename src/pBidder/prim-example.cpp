/**
 * @author Mike Bogochow
 * @version 2.5.0, Dec 8, 2015
 *
 * @file main.cpp
 *
 * pBidder main
 */

#include "defs.h"

// boost
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/graph/graph_utility.hpp>
using namespace boost;

#include <iostream>
#include <algorithm> // std::remove
#include <limits>

#define a 0
#define b 1
#define c 2
#define d 3
#define e 4
#define f 5
#define _g 6
#define h 7
#define _i 8


typedef std::vector<std::size_t> Path;

static const int __num_nodes = 9;
static const int __num_edges = 36;
static E __edges[__num_edges] = {
    E(a, b), E(a, c), E(a, d), E(a, e), E(a, f), E(a, _g), E(a, h), E(a, _i),
    E(b, c), E(b, d), E(b, e), E(b, f), E(b, _g), E(b, h), E(b, _i),
    E(c, d), E(c, e), E(c, f), E(c, _g), E(e, h), E(c, _i),
    E(d, e), E(d, f), E(d, _g), E(d, h), E(d, _i),
    E(e, f), E(e, _g), E(e, h), E(e, _i),
    E(f, _g), E(f, h), E(f, _i),
    E(_g, h), E(_g, _i),
    E(h, _i)
};
static int __weights[__num_edges] = {
    4,       8,       11,      8,      7,        4,       2,      9,
    14,      10,      2,       1,      6,        7,       4,
    8,       11,      8,       7,      4,        2,
    9,       14,      10,      2,      1,
    6,       7,       4,       8,
    11,      8,       7,
    4,       2,
    9 };

/**
 * Recurser function for get_path to do the search
 */
void get_path_helper(SpanningTree p, std::vector<std::size_t> &path,
    std::size_t curr)
{
  for (std::size_t i = 0; i != p.size(); i++)
  {
    // Skip over any nodes that don't have current node as parent
    // Therefore also skips previously visited nodes
    if (p[i] == curr && i != curr)
    {
      path.push_back(i);
      get_path_helper(p, path, i);
    }
  }
}

/**
 * Get path from spanning tree using depth-first search.
 *
 * @param p the spanning tree
 * @param path[out] the output path
 * @param curr the current index into the tree that is being processed
 */
void get_path(SpanningTree p, Path &path)
{
  std::size_t curr = 0;
  path.push_back(0); // default root of MST is 0th node
  get_path_helper(p, path, curr);
}

/**
 * Currently not a great solution.  Maybe look into storing the costs within
 * the path itself.  Also only works for fully connected graph.
 */
int get_path_cost(Graph g, Path path)
{
  int cost = 0;
  Path::iterator it = path.begin();
  int prevNode = *it++;
  for (; it != path.end(); it++)
  {
    int nextNode = *it;
    for (int i = 0; i < __num_edges; i++)
    {
      E edge = __edges[i];
      if (edge.first == prevNode)
      {
        if (edge.second == nextNode)
        {
          cost += __weights[i];
          break;
        }
      }
      else if (edge.first == nextNode)
      {
        if (edge.second == prevNode)
        {
          cost += __weights[i];
          break;
        }
      }
    }
    prevNode = nextNode;
  }
  return cost;
}

Graph get_subgraph(Graph g, std::vector<int> included_vertices)
{
  Graph sub(included_vertices.size());

  // Add all edges from original graph to subgraph which connect nodes
  // contained within the subgraph
  // O(E*V')
  for (int i = 0; i < __num_edges; i++)
  {
    int first = __edges[i].first;
    int second = __edges[i].second;
    int firstIndex = -1;
    int secondIndex = -1;
    int index = 0;

    for (std::vector<int>::iterator it = included_vertices.begin();
        it != included_vertices.end(); it++, index++)
    {

      if (firstIndex >= 0 && secondIndex >= 0)
        break;

      if (first == *it)
      {
        firstIndex = index;
        continue;
      }

      if (second == *it)
      {
        secondIndex = index;
        continue;
      }
    }

    if (firstIndex >= 0 && secondIndex >= 0)
    {
//      std::cerr << "(" << first << "," << second << "), v: " << "(" << firstIndex << "," << secondIndex << ")"/*<< *it */<< std::endl;
      add_edge(firstIndex, secondIndex, __weights[i], sub);
    }
  }

  return sub;
}

/**
 * Main
 */
int main(int argc, char **argv)
{
//  int weights[num_edges];
//
//  // Process command line
//  if (argc != num_edges + 1)
//  {
//    std::cerr << "Usage: need weight for all " << num_edges << " edges"
//        << std::endl;
//    return EXIT_FAILURE;
//  }
//
//  for (int i = 1, index = 0; i < argc; i++, index++)
//    __weights[index] = lexical_cast<int>(argv[i]);

  Graph g(__edges, __edges + __num_edges, __weights, __num_nodes);

  // TODO add my location to graph

  std::vector<int> allocated;
  // TODO add my location to allocated

//  Graph sub = get_subgraph(g, allocated);
//  std::cout << "sub1:" << std::endl;
//  print_graph(sub);
//  allocated.push_back(0);
//  sub = get_subgraph(g, allocated);
//  std::cout << "sub2:" << std::endl;
//  print_graph(sub);
//  allocated.push_back(1);
//  sub = get_subgraph(g, allocated);
//  std::cout << "sub3:" << std::endl;
//  print_graph(sub);
//
//  allocated.push_back(2);
//  allocated.push_back(h);
//  sub = get_subgraph(g, allocated);
//  std::cout << "sub4:" << std::endl;
//  print_graph(sub);
//  SpanningTree subtree(num_vertices(sub));
//  prim_minimum_spanning_tree(sub, &subtree[0]);
//  std::cout << "Spanning Tree:" << std::endl;
//  for (std::size_t i = 0; i != subtree.size(); ++i)
//  {
//    if (subtree[i] != i)
//      std::cout << "parent[" << i << "] = " << subtree[i] << std::endl;
//    else
//      std::cout << "parent[" << i << "] = no parent" << std::endl;
//  }


  Graph sub;
  std::vector<int> unallocated;
  unallocated.reserve(__num_nodes);

  int rtc = 0; // my cost
  for (int i = 0; i < __num_nodes; i++)
    unallocated.push_back(i);

  // each round of bidding
  for (int i = 0; i < __num_nodes; i++)
  {
    std::pair<int, int> currentBid = std::make_pair(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());

std::cerr << "~~~ Round " << i+1 << std::endl;
    // each unallocated target
    for (std::vector<int>::iterator t = unallocated.begin();
        t != unallocated.end(); t++)
    {
      std::vector<int> possibleAllocation;
      possibleAllocation.reserve(__num_nodes);
      possibleAllocation = allocated;
      possibleAllocation.push_back(*t);
std::cerr << "Adding " << *t << " for possible allocation" << std::endl;


      sub = get_subgraph(g, possibleAllocation);
print_graph(sub);
      SpanningTree tree(num_vertices(sub));
      prim_minimum_spanning_tree(sub, &tree[0]);
std::cout << "Spanning Tree:" << std::endl;
for (std::size_t i = 0; i != tree.size(); ++i)
{
  if (tree[i] != i)
    std::cout << "parent[" << i << "] = " << tree[i] << std::endl;
  else
    std::cout << "parent[" << i << "] = no parent" << std::endl;
}

      Path path;
      path.reserve(__num_nodes);
      get_path(tree, path);

      int cost = get_path_cost(g, path);
      int bid = cost - rtc;
std::cerr << "bid[" << i << "_" << *t << "]=" << bid << " (cost-rtc)=(" << cost << "-" << rtc << ")" << std::endl;

      if (currentBid.second > bid && bid >= 0)
        currentBid = std::make_pair(*t, bid);

std::cerr << "cur_bid=" << currentBid.first << ":" << currentBid.second << std::endl;
    }

    // submit current bid
    // get results

    // if win
    //   allocated.push_back(winning target);
    //   unallocated.erase(std::remove(vec.begin(), vec.end(), winning target), vec.end());
    //   update rtc

    int winner = unallocated.front();
    allocated.push_back(winner);
    unallocated.erase(std::remove(unallocated.begin(), unallocated.end(), winner), unallocated.end());
    rtc += currentBid.second;
  }

  sub = get_subgraph(g, allocated);
  SpanningTree tree(num_vertices(sub));
  prim_minimum_spanning_tree(sub, &tree[0]);

  // Output spanning tree
  std::cout << "Spanning Tree:" << std::endl;
  for (std::size_t i = 0; i != tree.size(); ++i)
  {
    if (tree[i] != i)
      std::cout << "parent[" << i << "] = " << tree[i] << std::endl;
    else
      std::cout << "parent[" << i << "] = no parent" << std::endl;
  }

  // Get the path from the tree
  Path path;
  path.reserve(__num_nodes);
  get_path(tree, path);
  int pathCost = get_path_cost(g, path);

  // Output the path
  std::cout << "Path:" << std::endl;
  for (std::vector<std::size_t>::iterator it = path.begin(); it != path.end(); it++)
    std::cerr << *it << std::endl;
  std::cerr << "cost: " << pathCost << std::endl;

  return EXIT_SUCCESS;
}
