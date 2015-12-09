

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

private:
  std::vector<int> path;
  weight_t *costs;

  Path(const size_t size);
};

#endif /* MBOGO_PATH_H_ */
