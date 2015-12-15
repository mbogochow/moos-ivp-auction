

#include "SpanningTree.h"
#include "defs.h"

#include <boost/graph/prim_minimum_spanning_tree.hpp>

#include <iostream>

SpanningTree::SpanningTree(const size_t size)
{
  tree.resize(size); // must be resize not reserve for prim's alg.
}

SpanningTree::~SpanningTree(void)
{

}

size_t
SpanningTree::size(void)
{
  return tree.size();
}

Vertex
SpanningTree::get(const size_t index)
{
  return tree.data()[index];
}

SpanningTree *
SpanningTree::fromGraph(UndirectedGraph *const g)
{
  SpanningTree *newTree = new SpanningTree(num_vertices(*g));
  prim_minimum_spanning_tree(*g, &newTree->tree[0]);
  return newTree;
}

void
SpanningTree::print(void) const
{
  for (size_t i = 0; i != tree.size(); ++i)
  {
    if (tree.data()[i] != i)
      std::cout << "parent[" << i << "] = " << tree.data()[i] << std::endl;
    else
      std::cout << "parent[" << i << "] = no parent" << std::endl;
  }
}

std::string
SpanningTree::toString(void) const
{
  std::ostringstream os;
  std::streambuf *coutbuf = std::cout.rdbuf();
  std::cout.rdbuf(os.rdbuf());

  print();

  std::cout.rdbuf(coutbuf);

  return os.str();
}
