

#ifndef MBOGO_PATH_H_
#define MBOGO_PATH_H_

#include "defs.h"
#include "SpanningTree.h"

class Path
{
public:
  ~Path(void);

  /**
   * Get path from spanning tree using depth-first search.
   *
   * @param tree the spanning tree
   */
  static Path * fromTree(SpanningTree *const tree);

  static Path * fromPath(std::vector<Vertex> path);

  static Path * fromPair(std::pair<Vertex, Vertex> path);

  mbogo_weight_t getTotalCost(Graph *g);

  std::vector<Vertex> * data(void);

private:
  std::vector<Vertex> path;
  size_t length;

  Path(const size_t size);
};

#endif /* MBOGO_PATH_H_ */
