/* Mike Bogochow */

#include "graph.h"

#include <assert.h>
#include <stdexcept>

Graph::Graph(void)
{
  id = 0;
  root = nullptr;
}

Graph::~Graph(void)
{
  if (root != nullptr)
  {
    std::list<struct edge *>::iterator edgeIter;
    std::list<struct node *>::iterator nodeIter;
    for (edgeIter = edges.begin(); edgeIter != edges.end(); ++edgeIter)
      deleteEdge(&edgeIter);
    for (nodeIter = nodes.begin(); nodeIter != nodes.end(); ++nodeIter)
    {
      struct node *node = *nodeIter;
      nodeIter = nodes.erase(nodeIter);
      delete node;
    }

    delete root;
  }
}

struct node *
Graph::getNodes(void) const
{
  return root;
}

std::list<struct node *> *
Graph::asList(void)
{
  // not great solution since exposes private object for modification
  return &nodes;
}

std::list<struct edge *> *
Graph::getEdges(void)
{
  // not great solution since exposes private object for modification
  return &edges;
}

struct node *
Graph::addNode()
{
  if (root == nullptr)
  {
    root = new struct node;
    initNode(root);
    nodes.push_back(root);
  }
  return root;
}

struct node *
Graph::addNode(struct node *connectedNode, const int cost)
{
  struct node *node;

  if (root == nullptr)
    node = addNode();
  else if (!contains(connectedNode))
    node = nullptr;
  else
  {
    node = new struct node;
    nodes.push_back(node);
    connectEdge(node, connectedNode, cost);
  }

  return node;
}

struct edge *
Graph::connectEdge(struct node *node1, struct node *node2, const int cost)
{
  struct edge *edge;

  if (!contains(node1) || !contains(node2))
    edge = nullptr;
  else if(connected(node1, node2))
    edge = nullptr;
  else
  {
    edge = new struct edge;
    edge->cost = cost;
    edge->node1 = node1;
    edge->node2 = node2;
    node1->edges.push_back(edge);
    node2->edges.push_back(edge);
    edges.push_back(edge);
  }

  return edge;
}

bool
Graph::disconnectEdge(struct node *node1, struct node *node2)
{
  bool ret = true;
  struct edge *edge;

  edge = getEdge(node1, node2);
  if (edge == nullptr)
    ret = false;
  else
    deleteEdge(&edge);

  return ret;
}

struct edge *
Graph::getEdge(struct node *node1, struct node *node2)
{
  struct edge *ret;

  if (!connected(node1, node2))
      ret = nullptr;
  else
  {
    // Find the edge and delete it
    std::list<struct edge *>::iterator edgeIter;
    for (edgeIter = node1->edges.begin(); edgeIter != node1->edges.end();
        ++edgeIter)
    {
      if (getOtherNode((struct edge *)*edgeIter, node1) == node2)
      {
        ret = *edgeIter;
        break;
      }
    }
  }

  return ret;
}

bool
Graph::disconnectEdge(struct edge *edge)
{
  bool ret = true;

  if (edge == nullptr || !contains(edge->node1) || !contains(edge->node2))
    ret = false;
  else
    deleteEdge(&edge);

  return ret;
}

void
Graph::deleteEdge(struct edge **edge)
{
  struct node *node1 = (*edge)->node1;
  struct node *node2 = (*edge)->node2;

  node1->edges.remove(*edge);
  node2->edges.remove(*edge);
  edges.remove(*edge);
  delete *edge;
}

std::list<struct edge *>::iterator
Graph::deleteEdge(std::list<struct edge *>::iterator *edgeIter)
{
  struct edge *edge = **edgeIter;
  struct node *node1 = edge->node1;
  struct node *node2 = edge->node2;
  std::list<struct edge *>::iterator ret;

  node1->edges.remove(edge);
  node2->edges.remove(edge);

  ret = edges.erase(*edgeIter);
  delete edge;
  return ret;
}

bool
Graph::contains(struct node * const node)
{
  bool ret = false;

  if (root != nullptr)
  {
    ret = DFS([&] (struct node *onode, struct edge *edge) {
      if (onode == node)
        return false; // stop searching if found
      return true;
    });
  }

  return ret;
}

bool
Graph::connected(struct node *node1, struct node *node2)
{
  bool ret = true;

  if (!contains(node1) || !contains(node2))
    ret = false;
  else
    ret = getEdge(node1, node2) != nullptr;

  return ret;
}

struct node *
Graph::getOtherNode(const struct edge * const edge,
    const struct node * const node)
{
  if (edge->node1 == node)
    return edge->node2;
  else if (edge->node2 == node)
    return edge->node1;

  return nullptr;
}

void
Graph::initNode(struct node *node)
{
  if (node == nullptr)
    throw new std::invalid_argument("Can't init null node");

  node->id = id++;
}

/**
 * Visit the given node during DFS.
 *
 * @param node the node to visit
 * @param find the node to find
 * @param found[out] whether the node was found
 * @param states the states of the nodes
 * @return true on success; false on failure (if a cycle is found)
 */
template<typename DFS_func>
static bool
DFS_visit(struct node * node, struct edge *edge,
    DFS_func dfs_func, search_state *states)
{
  bool ret = true;

  assert(node);

  if (states[node->id] == DISCOVERED)
    ret = false;
  else
  {
    states[node->id] = DISCOVERED;

    // Call the provided function for each visited node
    if ((ret = dfs_func(node, edge)) == true)
    {
      for (std::list<struct edge *>::iterator it = node->edges.begin();
          it != node->edges.end(); ++it)
      {
        struct node *pnode = Graph::getOtherNode((struct edge *)*it, node);
        assert(pnode != nullptr);

        if (states[pnode->id] == UNDISCOVERED)
        {
          // Recurse through remaining nodes
          if ((ret = DFS_visit(pnode, *it, dfs_func, states)) == false)
            break;
        }
      }

      if (ret)
        states[node->id] = FINISHED;
    }
  }

  return ret;
} /* DFS_visit */

/**
 * Perform a depth-first on the node list.
 *
 * @param head the head of the list to perform DFS on
 * @param DFS_func function to call for each visited node
 * @param data data to pass to DFS_func
 * @return true on success; false on failure
 */
template<typename DFS_func> bool
Graph::DFS(DFS_func dfs_func)
{
  bool ret = false;
  struct node *pnode;
  search_state *states;

  states = new search_state[nodes.size()];
  std::fill(states, states + nodes.size(), UNDISCOVERED);

  for (std::list<struct edge *>::iterator it = root->edges.begin();
      it != root->edges.end(); ++it)
  {
    pnode = getOtherNode((struct edge *)*it, root);
    assert(pnode != nullptr);

    if (states[pnode->id] == UNDISCOVERED)
    {
      if ((ret = DFS_visit(pnode, *it, dfs_func, states)) == false)
        break;
    }
  }

  delete states;

  return ret;
} /* DFS */
