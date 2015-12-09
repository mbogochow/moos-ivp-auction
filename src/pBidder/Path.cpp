

#include "Path.h"
#include "SpanningTree.h"

Path::Path(size_t size)
{
  costs = new weight_t[size];
  path.reserve(size);
}

Path::~Path(void)
{
  delete costs;
  path.clear();
}

/**
 * Recurser function for fromTree to do the search
 */
static void fromTreeHelper(SpanningTree * const tree,
    std::vector<int> * const path, std::size_t curr)
{
  for (std::size_t i = 0; i != tree->size(); i++)
  {
    // Skip over any nodes that don't have current node as parent
    // Therefore also skips previously visited nodes
    if (tree->get(i) == curr && i != curr)
    {
      path->push_back(i);
      fromTreeHelper(tree, path, i);
    }
  }
}

Path *
Path::fromTree(SpanningTree *const tree)
{
  Path * const newPath = new Path(tree->size());

  std::size_t curr = 0;
  newPath->path.push_back(0); // default root of MST is 0th node
  fromTreeHelper(tree, &newPath->path, curr);

  return newPath;
}
