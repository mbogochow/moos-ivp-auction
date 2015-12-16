
#ifndef MBOGO_DEFS_H_
#define MBOGO_DEFS_H_

#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
    boost::property<boost::vertex_distance_t, int>,
    boost::property<boost::edge_weight_t, int>> UndirectedGraph;
typedef boost::graph_traits<UndirectedGraph>::vertex_descriptor Vertex;
typedef std::pair<Vertex, Vertex> Edge;
typedef boost::graph_traits<UndirectedGraph>::edge_descriptor EdgeDescriptor;
typedef boost::graph_traits<UndirectedGraph>::edge_iterator EdgeIterator;

typedef boost::integer_range<Vertex> VertexSet;

typedef int mbogo_weight_t;

const Vertex MAX_VERTEX = std::numeric_limits<Vertex>::max();
const mbogo_weight_t MAX_WEIGHT = std::numeric_limits<mbogo_weight_t>::max();

typedef std::pair<double, double> Point;

#endif /* MBOGO_DEFS_H_ */
