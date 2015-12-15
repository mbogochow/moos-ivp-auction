
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

typedef std::pair<double, double> Loc;

#define _a 0
#define _b 1
#define _c 2
#define _d 3
#define _e 4
#define _f 5
#define _g 6
#define _h 7
#define _i 8

extern const size_t __num_nodes;
extern const size_t __num_edges;
extern Edge __edges[];
extern int __weights[];
extern Loc __locations[];

#endif /* MBOGO_DEFS_H_ */
