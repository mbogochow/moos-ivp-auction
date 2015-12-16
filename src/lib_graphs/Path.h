

#ifndef MBOGO_PATH_H_
#define MBOGO_PATH_H_

#include "defs.h"
#include "SpanningTree.h"

class Path
{
public:
  ~Path(void);

  /**
   * Get path from a spanning tree using depth-first search.
   */
  static Path * fromTree(SpanningTree *const tree);

  /** Construct a path from a vector. */
  static Path * fromPath(std::vector<Vertex> path);

  /** Construct a path from the given pair.  The path size will be 2. */
  static Path * fromPair(std::pair<Vertex, Vertex> path);

  /**
   * Converts the values along this path to the matching values in the given
   * path.  i.e. this->path[i] = path[this->path[i]] for all i in this.
   */
  void convertPath(Vertex *path);

  /**
   * Get the cost to traverse the path given the weights in the given graph.
   */
  mbogo_weight_t getTotalCost(UndirectedGraph *g);

  /** Get pointer to the underlying path. */
  std::vector<Vertex> * data(void);

  /** Print the path to std:cout */
  void print(void) const;

  /**
   * Convert the path to a string with the same formatting as the print
   * function.
   */
  std::string toString(void) const;

  /**
   * Get the locations of the vertices along the path. Assumes locations size
   * equal to size of path.
   *
   * @param locations the locations of the absolute vertices
   * @param locs[out] the locations of the vertices in the path in order of
   *                  the path
   */
  void getLocations(Loc *locations, Loc *locs);

  /** Get the length of the path in number of vertices. */
  size_t getLength(void) { return length; }

private:
  std::vector<Vertex> path;
  size_t length;

  Path(const size_t size);
};

#endif /* MBOGO_PATH_H_ */
