/**
 * @author Mike Bogochow
 * @version 2.5.0, Dec 8, 2015
 *
 * @file main.cpp
 *
 * pBidder main
 */

#include <boost/config.hpp>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost;

#define a 0
#define b 1
#define c 2
#define d 3
#define e 4
#define f 5
#define _g 6
#define h 7
#define _i 8


typedef adjacency_list<vecS, vecS, undirectedS,
  property<vertex_distance_t, int>, property<edge_weight_t, int>> Graph;

typedef std::vector<graph_traits<Graph>::vertex_descriptor> SpanningTree;

/**
 * Recurser function fro get_path to do the search
 */
void get_path_helper(SpanningTree p, std::vector<std::size_t> &path, std::size_t curr)
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
void get_path(SpanningTree p, std::vector<std::size_t> &path)
{
  std::size_t curr = 0;
  path.push_back(0); // default root of MST is 0th node
  get_path_helper(p, path, curr);
}

/**
 * Main
 */
int main(int argc, char **argv)
{
  typedef std::pair<int, int> E;
  const int num_nodes = 9;
  const int num_edges = 14;
  E edges[num_edges] = {
      E(a, b), E(a, h), E(b, h), E(b, c), E(c, d), E(c, f), E(c, _i),
      E(d, e), E(d, f), E(e, f), E(f, _g), E(_g, h), E(_g, _i), E(h, _i)
  };
//  int weights[] = { 4, 8, 11, 8, 7, 4, 2, 9, 14, 10, 2, 1, 6, 7 };
  int weights[num_edges];

  // Process command line
  if (argc != num_edges + 1)
  {
    std::cerr << "Usage: need weight for all " << num_edges << " edges"
        << std::endl;
    return EXIT_FAILURE;
  }

  for (int i = 1, index = 0; i < argc; i++, index++)
    weights[index] = lexical_cast<int>(argv[i]);

  Graph g(edges, edges + num_edges, weights, num_nodes);
  SpanningTree tree(num_vertices(g));

  prim_minimum_spanning_tree(g, &tree[0]);

  // Output spanning tree
  for (std::size_t i = 0; i != tree.size(); ++i)
  {
    if (tree[i] != i)
      std::cout << "parent[" << i << "] = " << tree[i] << std::endl;
    else
      std::cout << "parent[" << i << "] = no parent" << std::endl;
  }

  // Get the path from the tree
  std::vector<std::size_t> path;
  path.reserve(num_nodes);
  get_path(tree, path);

  // Output the path
  std::cout << "Path:" << std::endl;
  for (std::vector<std::size_t>::iterator it = path.begin(); it != path.end(); it++)
    std::cerr << *it << std::endl;

  return EXIT_SUCCESS;
}
