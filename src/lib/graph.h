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
  Graph(void);
  ~Graph(void);

  struct node * addNode(void);
  struct node * addNode(struct node *connectedNode, const int cost);
  struct edge * connectEdge(struct node *node1, struct node *node2,
      const int cost);
  bool disconnectEdge(struct node *node1, struct node *node2);
  bool disconnectEdge(struct edge *edge);
  struct node * getNodes() const;
  std::list<struct node *> * asList(void);
  std::list<struct edge *> * getEdges(void);
  struct edge * getEdge(struct node *node1, struct node *node2);
  static struct node * getOtherNode(const struct edge * const edge,
      const struct node * const node);

private:
  struct node *root;
  unsigned int id;
  std::list<struct node *> nodes;
  std::list<struct edge *> edges;

  bool contains(struct node *node);
  bool connected(struct node *node1, struct node *node2);
  void initNode(struct node *node);
  template<typename DFS_func> bool DFS(DFS_func dfs_func);
//  template<typename DFS_func> bool DFS_visit(struct node * const node,
//      DFS_func dfs_func, void *data, search_state * const states);
  std::list<struct edge *>::iterator deleteEdge(
      std::list<struct edge *>::iterator *);
  void deleteEdge(struct edge **edge);
};

#endif /* MBOGO_GRAPH_H_ */
