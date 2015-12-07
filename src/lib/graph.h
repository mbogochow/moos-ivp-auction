// Mike Bogochow

#ifndef MBOGO_GRAPH_H_
#define MBOGO_GRAPH_H_

#include<list>

struct node
{
  unsigned int id;
  std::list<struct edge *> edges;
};

struct edge
{
  struct node *node1;
  struct node *node2;
  double cost;
};

typedef enum search_state
{
  UNDISCOVERED, DISCOVERED, FINISHED
} search_state;

class Graph
{
public:
  Graph();
  ~Graph();

  struct node * addNode();
  struct node * addNode(struct node *connectedNode, const int cost);
  struct edge * connectEdge(struct node *node1, struct node *node2, const int cost);
  bool disconnectEdge(struct node *node1, struct node *node2);
  bool disconnectEdge(struct edge *edge);
  struct node * getNodes();
  struct edge * getEdge(struct node *node1, struct node *node2);

private:
  void (*DFS_func)(void *);

  struct node *root;
  int nodeCount;
  unsigned int id;

  bool contains(struct node *node);
  bool connected(struct node *node1, struct node *node2);
  void initNode(struct node *node);
  struct node * getOtherNode(const struct edge * const edge, const struct node * const node);
  bool DFS(struct node * const head);
  bool DFS_visit(struct node * const node, struct node * const find,
      bool &found, search_state * const states);
  void deleteEdge(struct edge **edge);
};

#endif /* MBOGO_GRAPH_H_ */
