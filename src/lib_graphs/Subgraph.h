

#ifndef MBOGO_SUBGRAPH_H_
#define MBOGO_SUBGRAPH_H_

#include "defs.h"
#include "Graph.h"

class Subgraph : public Graph
{
public:
//  Subgraph(Edge *edges, size_t numEdges, mbogo_weight_t *weights, size_t numNodes);
//  Subgraph(Graph *other);
  ~Subgraph(void);

  static Subgraph * fromGraph(Graph *g, std::vector<Vertex> coveredNodes);

  Vertex getParentIndex(Vertex index);

  Vertex * getParentIndices(void);

protected:
  Vertex *parentIndices;

  Subgraph(size_t numNodes);
};

#endif /* MBOGO_SUBGRAPH_H_ */
