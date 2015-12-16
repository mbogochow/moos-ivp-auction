

#include "Graph.h"
#include "Subgraph.h"

#include <boost/graph/graph_utility.hpp>

//Subgraph::Subgraph(Edge *edges, size_t numEdges, mbogo_weight_t *weights, size_t numNodes)
//  : Graph(edges, numEdges, weights, numNodes)
//{
//
//}
//
//Subgraph::Subgraph(Graph *other)
//  : Graph(other)
//{
//
//}

Subgraph::~Subgraph(void)
{
  delete parentIndices;
}

Subgraph::Subgraph(size_t numNodes)
  : Graph(numNodes)
{
  parentIndices = new Vertex[numNodes];
}

Vertex
Subgraph::getParentIndex(const Vertex index)
{
  return parentIndices[index];
}

Vertex *
Subgraph::getParentIndices(void)
{
  // TODO: Dangerous
  return parentIndices;
}

Subgraph *
Subgraph::fromGraph(Graph *g, std::vector<Vertex> coveredNodes)
{
  Subgraph *sub = new Subgraph(coveredNodes.size());

  const Vertex def = MAX_VERTEX; // Default vertex val
  size_t numEdges = g->edges.size();

  // Add all edges from original graph to subgraph which connect nodes
  // contained within the subgraph
  // O(E*V')
  for (size_t i = 0; i < numEdges; i++)
  {
    Vertex first = g->edges.data()[i].first;
    Vertex second = g->edges.data()[i].second;
    Vertex firstIndex = def;
    Vertex secondIndex = def;
    Vertex index = 0;

    for (std::vector<Vertex>::iterator it = coveredNodes.begin();
        it != coveredNodes.end(); it++, index++)
    {
      if (firstIndex != def && secondIndex != def)
        break;

      if (first == *it)
      {
        firstIndex = index;
        continue;
      }

      if (second == *it)
      {
        secondIndex = index;
        continue;
      }
    }

    if (firstIndex != def && secondIndex != def)
    {
//      std::cerr << "(" << first << "," << second << "), v: " << "(" << firstIndex << "," << secondIndex << ")"/*<< *it */<< std::endl;
      boost::add_edge(firstIndex, secondIndex, g->weights.data()[i], *(sub->g));
      sub->addEdge(std::make_pair(firstIndex, secondIndex),
          g->weights.data()[i]);

      sub->parentIndices[firstIndex] = first;
      sub->parentIndices[secondIndex] = second;
    }
  }

  return sub;
}
