

#include "Path.h"
#include "SpanningTree.h"

#include <iostream>

Path::Path(const size_t size)
{
  length = size;
  path.reserve(length);
}

Path::~Path(void)
{
//  path.clear();
}

/**
 * Recurser function for fromTree to do the search
 */
static void fromTreeHelper(SpanningTree * const tree,
    std::vector<Vertex> * const path, std::size_t curr)
{
  for (std::size_t i = 0; i != tree->size(); i++)
  {
    // Skip over any nodes that don't have current node as parent.
    // Therefore also skips previously visited nodes.
    // Note: tree.get gives the parent of node
    if (i != curr && tree->get(i) == curr)
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

Path *
Path::fromPath(std::vector<Vertex> path)
{
  Path *newPath = new Path(path.size());
  newPath->path = path;
  return newPath;
}

Path *
Path::fromPair(std::pair<Vertex, Vertex> path)
{
  Path *newPath = new Path(2);
  newPath->path.push_back(path.first);
  newPath->path.push_back(path.second);
  return newPath;
}

mbogo_weight_t
Path::getTotalCost(UndirectedGraph *const g)
{
  mbogo_weight_t cost = 0;

  if (length == 0 || length == 1)
    return 0;

  EdgeDescriptor e;
  bool found;
  typename boost::property_map<UndirectedGraph, boost::edge_weight_t >::type
    weight = boost::get(boost::edge_weight, *g);
  std::vector<Vertex>::iterator it = path.begin();
  Vertex prevNode = *it++;

  for (; it != path.end(); it++)
  {
    tie(e, found) = edge(prevNode, *it, *g);
//    std::cout << "weight(" << prevNode << "," << *it << "):" << boost::get(weight, e) << std::endl;
    cost += boost::get(weight, e);
    prevNode = *it;
  }

  return cost;
}

void
Path::getLocations(Loc *locations, Loc *locs)
{
  for (size_t i = 0; i < length; i++)
    locs[i] = locations[path.data()[i]];
}

void
Path::convertPath(Vertex *path)
{
  std::vector<Vertex> tempPath;
  tempPath.resize(length);
  for (size_t i = 0; i < length; i++)
    tempPath[i] = path[this->path[i]];
  this->path = tempPath;
}

std::vector<Vertex> *
Path::data(void)
{
  return &path;
}

void
Path::print(void) const
{
  for (std::vector<Vertex>::const_iterator it = path.begin(); it != path.end();
      it++)
  {
    std::cout << *it << std::endl;
  }
}

std::string
Path::toString(void) const
{
  std::ostringstream os;
  std::streambuf *coutbuf = std::cout.rdbuf();
  std::cout.rdbuf(os.rdbuf());

  print();

  std::cout.rdbuf(coutbuf);

  return os.str();
}
