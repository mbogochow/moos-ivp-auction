

#ifndef MBOGO_GRAPH_H_
#define MBOGO_GRAPH_H_

#include "defs.h"

#include <boost/graph/graph_utility.hpp>

class Graph
{
public:
  Graph(Edge *edges, size_t numEdges, mbogo_weight_t *weights, size_t numNodes);
  Graph(Graph *other);
  ~Graph(void);

  UndirectedGraph * getGraph(void) const { return g; }
  size_t getNumNodes(void) const { return numNodes; }
  size_t getNumEdges(void) const { return edges.size(); }
  void print(void) const { print_graph(*g); }

  Graph * getSubgraph(std::vector<Vertex> includedVertices);

private:
  UndirectedGraph *g;
  size_t numNodes;
  std::vector<Edge> edges;
  std::vector<mbogo_weight_t> weights;

  Graph(size_t numNodes);
  void assignDimensions(UndirectedGraph *graph);
  /**
   * Assumes that the size of the parameter arrays is at least edges.capacity()
   */
  void assignEdgeVals(Edge *edges, mbogo_weight_t *weights);
  void addEdge(Edge edge, mbogo_weight_t weight);
};

#endif /*MBOGO_GRAPH_H_*/
