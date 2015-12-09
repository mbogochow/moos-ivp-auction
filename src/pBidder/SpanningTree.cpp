

#include "SpanningTree.h"
#include "defs.h"

#include <boost/graph/prim_minimum_spanning_tree.hpp>

SpanningTree::SpanningTree(const size_t size)
{
  tree.reserve(size);
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

SpanningTree *
SpanningTree::fromGraph(Graph *g)
{
  SpanningTree *newTree = new SpanningTree(num_vertices(*g));
  prim_minimum_spanning_tree(*g, &newTree->tree[0]);
  return newTree;
}
