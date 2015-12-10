

#ifndef MBOGO_DEFS_H_
#define MBOGO_DEFS_H_

#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
    boost::property<boost::vertex_distance_t, int>,
    boost::property<boost::edge_weight_t, int>> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef std::pair<Vertex, Vertex> Edge;
typedef typename boost::graph_traits<Graph>::edge_descriptor EdgeDescriptor;

typedef boost::integer_range<Vertex> VertexSet;

typedef int mbogo_weight_t;

#endif /* MBOGO_DEFS_H_ */
