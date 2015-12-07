/* Mike Bogochow */

#include "graph.h"

#include <assert.h>
#include <stdexcept>

Graph::Graph()
{
  nodeCount = 0;
  id = 0;

  root = nullptr;
}

Graph::~Graph()
{
  if (root != nullptr)
  {


    delete root;
  }
}

struct node *
Graph::getNodes()
{
  return root;
}

struct node *
Graph::addNode()
{
  if (root == nullptr)
  {
    root = new struct node;
    initNode(root);

    assert(nodeCount == 0);
    nodeCount += 1;
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
    nodeCount += 1;
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

void deleteEdge(struct edge **edge)
{
  struct node *node1 = (*edge)->node1;
  struct node *node2 = (*edge)->node2;

  node1->edges.remove(*edge);
  node2->edges.remove(*edge);
  delete *edge;
}

bool
Graph::contains(struct node * const node)
{
  bool ret = true;

  if (root == nullptr)
    ret = false;
  else
    ret = DFS(node);

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
Graph::getOtherNode(const struct edge * const edge, const struct node * const node)
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
bool
Graph::DFS_visit(struct node * const node, struct node * const find,
    bool &found, search_state * const states)
{
  bool ret = true;
  struct node *pnode;

  assert(node);

  if (states[node->id] == DISCOVERED)
    ret = false;
  else
  {
    states[node->id] = DISCOVERED;
    for (std::list<struct edge *>::iterator it = node->edges.begin();
        it != node->edges.end(); ++it)
    {
      pnode = getOtherNode((struct edge *)*it, node);
      assert(pnode != nullptr);

      if (states[pnode->id] == UNDISCOVERED)
      {
        if (pnode == find)
        {
          found = true;
          break;
        }

        if ((ret = DFS_visit(pnode, find, found, states)) == false)
          break;
      }
    }
    if (ret)
      states[node->id] = FINISHED;
  }

  return ret;
} /* DFS_visit */

/**
 * Perform a depth-first on the node list.  Topological sort output to out.
 *
 * @param head the head of the list to perform DFS on
 * @param states the states of the nodes
 * @return true on success; false on failure
 */
bool
Graph::DFS(struct node * const node)
{
  bool ret = false;
  struct node *pnode;
  search_state *states;

  states = new search_state[nodeCount];
  std::fill(states, states + nodeCount, UNDISCOVERED);

  for (std::list<struct edge *>::iterator it = root->edges.begin();
      it != root->edges.end(); ++it)
  {
    pnode = getOtherNode((struct edge *)*it, root);
    assert(pnode != nullptr);

    if (states[pnode->id] == UNDISCOVERED)
    {
      if (DFS_visit(pnode, node, ret, states) == false || ret == true)
        break;
    }
  }

  delete states;

  return ret;
} /* DFS */
