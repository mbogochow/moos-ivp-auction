

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

  /** Get pointer to the underlying graph */
  UndirectedGraph * getGraph(void) const { return g; }

  /** Get number of vertices in graph */
  size_t getNumVertices(void) const { return numNodes; }

  /** Get number of edges in graph */
  size_t getNumEdges(void) const { return edges.size(); }

  /** Prints the graph to std::cout */
  void print(void) const { print_graph(*g); }

  /**
   * Get a subgraph of this graph covering the given vertices.
   */
  Graph * getSubgraph(std::vector<Vertex> includedVertices);

  /**
   * Converts the graph to a string with the same format as the print function.
   */
  std::string toString(void) const;

protected:
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

  friend class Subgraph;
};

#endif /*MBOGO_GRAPH_H_*/
