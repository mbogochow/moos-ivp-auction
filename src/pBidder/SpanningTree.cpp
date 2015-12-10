

#include "SpanningTree.h"
#include "defs.h"

#include <boost/graph/prim_minimum_spanning_tree.hpp>

SpanningTree::SpanningTree(const size_t size)
{
  tree.resize(size); // must be resize not reserve for prim's alg.
  weights.reserve(size);
}

SpanningTree::~SpanningTree(void)
{

}

size_t
SpanningTree::size(void)
{
  return tree.size();
}

size_t
SpanningTree::get(const size_t index)
{
  return tree.data()[index];
}

mbogo_weight_t
SpanningTree::getWeight(const size_t index)
{
  return weights.data()[index];
}

SpanningTree *
SpanningTree::fromGraph(Graph *const g)
{
  SpanningTree *newTree = new SpanningTree(num_vertices(*g));
  prim_minimum_spanning_tree(*g, &newTree->tree[0]);



  // add weights from graph edges
//  boost::graph_traits<Graph>::edge_iterator ei, ei_end;
////  boost::property_map<Graph, boost::edge_weight_t>::const_type weight = boost::get(
////      boost::edge_weight, *g);
//  boost::property_map<Graph, boost::edge_weight_t>::type weight = boost::get(
//      boost::edge_weight, *g);
//  for (tie(ei, ei_end) = boost::edges(*g); ei != ei_end; ++ei)
//  {
//    std::cout << "(" << source(*ei, *g) << "," << target(*ei, *g) << ")="
//        << boost::get(weight, ei) << std::endl;
//  }

  return newTree;
}
