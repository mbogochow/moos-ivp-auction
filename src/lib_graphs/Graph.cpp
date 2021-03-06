

#include "Graph.h"
#include "Subgraph.h"

#include <assert.h>

Graph::Graph(Edge * const edges, const size_t numEdges,
    mbogo_weight_t * const weights, const size_t numNodes)
{
  g = new UndirectedGraph(edges, edges + numEdges, weights, numNodes);

  assignDimensions(g);
  assignEdgeVals(edges, weights);

  assert(this->numNodes == numNodes);
}

Graph::Graph(const size_t numNodes)
{
  this->numNodes = numNodes;
  g = new UndirectedGraph(numNodes);
}

Graph::Graph(Graph * const other)
{
  assignDimensions(other->g);
  assignEdgeVals(other->edges.data(), other->weights.data());

  this->g = new UndirectedGraph(this->edges.data(),
      this->edges.data() + this->edges.size(), this->weights.data(),
      this->numNodes);
}

Graph::~Graph(void)
{
  delete g;
}

void
Graph::assignDimensions(UndirectedGraph * const graph)
{
  size_t numEdges = num_edges(*graph);
  this->numNodes = num_vertices(*graph);

  this->edges.reserve(numEdges);
  this->weights.reserve(numEdges);
}

void
Graph::assignEdgeVals(Edge * const edges, mbogo_weight_t * const weights)
{
  size_t numEdges = this->edges.capacity();
  for (size_t i = 0; i < numEdges; i++)
  {
    this->edges.push_back(edges[i]);
    this->weights.push_back(weights[i]);
  }
}

void
Graph::addEdge(Edge edge, mbogo_weight_t weight)
{
  edges.push_back(edge);
  weights.push_back(weight);
}

Graph *
Graph::getSubgraph(std::vector<Vertex> includedVertices)
{
  return Subgraph::fromGraph(this, includedVertices);
}

// TODO move functionality to a util function with lambda
std::string
Graph::toString(void) const
{
  std::ostringstream os;
  std::streambuf *coutbuf = std::cout.rdbuf();
  std::cout.rdbuf(os.rdbuf());

  print();

  std::cout.rdbuf(coutbuf);

  return os.str();
}
