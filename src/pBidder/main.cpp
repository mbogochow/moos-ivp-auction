/**
 * @author Mike Bogochow
 * @version 2.4.0, Dec 8, 2015
 *
 * @file main.cpp
 *
 * pBidder main
 */

#include "Bidder.h"

#include <vector>
#include <iostream>

// Boost
#include <boost/graph/graph_traits.hpp>
//#include <boost/graph/adjacency_list.hpp> // for customizable graphs
//#include <boost/graph/undirected_graph.hpp>// A subclass to provide reasonable arguments to adjacency_list for a typical undirected graph
//#include <boost/graph/prim_minimum_spanning_tree.hpp>
////#include <boost/config.hpp>
//#include <boost/property_map/property_map.hpp>

#include <boost/config.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

int main(int argc, char **argv)
{
  int ret = EXIT_SUCCESS;

//  typedef boost::undirected_graph<> Graph;

  typedef boost::property<boost::edge_weight_t, float> EdgeWeightProperty;
  typedef boost::undirected_graph<boost::property<boost::vertex_distance_t, int>, EdgeWeightProperty> Graph;

  Graph G;

  Graph::vertex_descriptor a = G.add_vertex();
  Graph::vertex_descriptor b = G.add_vertex();
  Graph::vertex_descriptor c = G.add_vertex();
  Graph::vertex_descriptor d = G.add_vertex();
  Graph::vertex_descriptor e = G.add_vertex();
  Graph::vertex_descriptor f = G.add_vertex();
  Graph::vertex_descriptor g = G.add_vertex();
  Graph::vertex_descriptor h = G.add_vertex();
  Graph::vertex_descriptor i = G.add_vertex();


  EdgeWeightProperty w_ab = 4;
  G.add_edge(a, b, w_ab);
  EdgeWeightProperty w_ah = 8;
  G.add_edge(a, h, w_ah);
  EdgeWeightProperty w_bc = 8;
  G.add_edge(b, c, w_bc);
  EdgeWeightProperty w_bh = 11;
  G.add_edge(b, h, w_bh);
  EdgeWeightProperty w_ci = 2;
  G.add_edge(c, i, w_ci);
  EdgeWeightProperty w_cf = 4;
  G.add_edge(c, f, w_cf);
  EdgeWeightProperty w_cd = 7;
  G.add_edge(c, d, w_cd);
  EdgeWeightProperty w_de = 9;
  G.add_edge(d, e, w_de);
  EdgeWeightProperty w_df = 14;
  G.add_edge(d, f, w_df);
  EdgeWeightProperty w_ef = 10;
  G.add_edge(e, f, w_ef);
  EdgeWeightProperty w_ih = 7;
  G.add_edge(i, h, w_ih);
  EdgeWeightProperty w_ig = 6;
  G.add_edge(i, g, w_ig);
  EdgeWeightProperty w_hg = 1;
  G.add_edge(h, g, w_hg);
  EdgeWeightProperty w_gf = 2;
  G.add_edge(g, f, w_gf);


  typedef boost::property_map<Graph, boost::edge_weight_t>::type EdgeWeightMap;
  EdgeWeightMap edgeWeightMap = get(boost::edge_weight, G);

  std::vector<boost::graph_traits<Graph>::vertex_descriptor> p(num_vertices(G));

  boost::prim_minimum_spanning_tree(G, &p[0]);

  for (std::size_t i = 0; i != p.size(); ++i)
  {
//    if (p[i] != i)
      std::cout << "parent[" << i << "] = " << p[i] << std::endl;
//    else
//      std::cout << "parent[" << i << "] = no parent" << std::endl;
  }

  return ret;
}
