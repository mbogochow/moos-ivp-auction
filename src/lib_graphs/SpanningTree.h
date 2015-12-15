

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

  /** Get the parent of the vertex at the given index in the tree. */
  Vertex get(size_t index);

  /** Prints the tree to std::cout */
  void print(void) const;

  /**
   * Convert the tree to a string with the same formatting as the print
   * function.
   */
  std::string toString(void) const;

private:
  std::vector<Vertex> tree;

  SpanningTree(size_t size);
};

#endif /*MBOGO_SPANNINGTREE_H_*/
