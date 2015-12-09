

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

private:
  typedef std::vector<boost::graph_traits<Graph>::vertex_descriptor> SpanningTree_t;
  SpanningTree_t tree;

  SpanningTree(size_t size);
};

#endif /*MBOGO_SPANNINGTREE_H_*/
