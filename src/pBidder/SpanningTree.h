

#ifndef MBOGO_SPANNINGTREE_H_
#define MBOGO_SPANNINGTREE_H_

#include "defs.h"

class SpanningTree
{
public:
  ~SpanningTree(void);

  size_t size(void);

  static SpanningTree * fromGraph(Graph *g);

  size_t get(size_t index);

  /**
   * Get the weight of traveling to the node at the given index from the
   * node's parent according to the original graph.
   */
  mbogo_weight_t getWeight(size_t index);

private:
  std::vector<Vertex> tree;
  std::vector<mbogo_weight_t> weights;

  SpanningTree(size_t size);
};

#endif /*MBOGO_SPANNINGTREE_H_*/
