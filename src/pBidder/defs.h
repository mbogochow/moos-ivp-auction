

#ifndef MBOGO_DEFS_H_
#define MBOGO_DEFS_H_

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/graph_utility.hpp>

typedef int weight_t;

typedef std::pair<int, int> E;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
    boost::property<boost::vertex_distance_t, int>,
    boost::property<boost::edge_weight_t, int>> Graph;

typedef boost::integer_range<
    boost::graph_traits<Graph>::vertex_descriptor> VertexSet;

#endif /* MBOGO_DEFS_H_ */
