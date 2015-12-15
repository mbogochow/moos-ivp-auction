

#ifndef MBOGO_SPANNINGTREE_H_
#define MBOGO_SPANNINGTREE_H_

#include "defs.h"

class SpanningTree
{
public:
  ~SpanningTree(void);

  /** Construct a spanning tree from the given graph using Prim's Algorithm. */
  static SpanningTree * fromGraph(UndirectedGraph *g);

  /** Get the size of the tree (the number of vertices) */
  size_t size(void);

  /** Get the vertex at the given index in the tree. */
  Vertex get(size_t index);

  /**
   * Get the weight of traveling to the node at the given index from the
   * node's parent according to the original graph.
   */
  mbogo_weight_t getWeight(size_t index);

  /** Prints the tree to std::cout */
  void print(void) const;

  /**
   * Convert the tree to a string with the same formatting as the print
   * function.
   */
  std::string toString(void) const;

private:
  std::vector<Vertex> tree;
  std::vector<mbogo_weight_t> weights;

  SpanningTree(size_t size);
};

#endif /*MBOGO_SPANNINGTREE_H_*/
