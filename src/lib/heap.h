/**
 * @author Mike Bogochow
 * @version 3.2.0, Dec 7, 2015
 *
 * @file heap.h
 *
 * Array based heap.
 */

#ifndef MBOGO_HEAP_H_
#define MBOGO_HEAP_H_

#include "graph.h"

#include <vector>
#include <cstring>

//#ifndef HEAP_DEBUG
//#define HEAP_DEBUG 0
//#endif

#define DBG_HEAP_LVL(LVL) defined(HEAP_DEBUG) && LVL <= HEAP_DEBUG

#define INIT_HEAP_SIZE 100

struct heap_node
{
  float value;

  int index;

  void *data;
};

inline size_t get_left_child_index(const size_t parent_index)
{
  return 2 * parent_index + 1;
} /* get_left_child_index */

inline size_t get_right_child_index(const size_t parent_index)
{
  return 2 * parent_index + 2;
} /* get_right_child_index */

inline size_t get_parent_index(const size_t child_index)
{
  return (child_index - 1) / 2;
} /* get_parent_index */

inline bool is_valid_index(const size_t size, const size_t index)
{
  return index >= 0 && index < size;
} /* is_valid_index */

/**
 * Represents a heap
 */
class Heap
{
public:
  Heap(void);
  ~Heap(void);

  /**
   * Convert the given array into a heap.
   *
   * @param ary the input array which should be turned into a heap
   * @param ary_len the length of the array.  Function does nothing if this value
   * is less than or equal to 0.
   * @return 0 on success or 1 on failure or if the value of {@param ary_len} is
   * less than or equal to 0
   */
  bool build_min_heap(struct heap_node* const ary, const size_t ary_len);

  /**
   * Pull the element at the given index up the tree until the invariant has
   * been reestablished.
   *
   * @param index the index of the element to pull up the tree
   * @return 0 on success or 1 on failure
   */
  bool pull_up(const size_t index);

  /**
   * Push the element at the given index down the tree until the invariant has
   * been reestablished
   *
   * @param index the index of the element to push down the tree
   * @return 0 on success or 1 on failure
   */
  bool push_down(const size_t index);

  /**
   * Is the node at index1 before the node at index2 in terms of the rules of
   * the heap?
   *
   * @param index1 the index of the first node
   * @param index2 the index of the second node
   * @return 1 if the node at index1 should come before the node at index2;
   * 0 otherwise
   */
  bool is_smaller(const size_t index1, const size_t index2) const;

  bool is_larger(const size_t index1, const size_t index2) const;

  /**
   * Get the lesser of the two indeces.  This is determined by the is_before
   * function.
   *
   * @param index1 the first index
   * @param index2 the second index
   * @return the lesser of the two indeces given the heap's rules
   */
  bool get_lesser_index(const size_t index1, const size_t index2) const;

  /**
   * Insert the element into the heap.
   *
   * @param element the element to insert into the heap
   * @return 0 on success or 1 on failure
   */
  bool insert(struct heap_node * const element);

  /**
   * Extract the smallest value of the heap.
   *
   * @param min[out] the extracted value
   * @return Returns 0 on success; != 0 if the extract failed or if there were
   * no elements in the heap.
   */
  bool extract_min(struct heap_node **min);

  /**
   * Resize the heap.
   *
   * @return 0 on success or 1 on failure (the heap will no longer be valid and
   * so should be cleaned)
   */
  bool resize(void);

  /**
   * Exchange the element at index1 of the heap with the element at index2 of
   * the heap.
   *
   * @param heap the heap whose elements should be exchanged
   * @param index1 the index of the first element
   * @param index2 the index of the second element
   * @return 0 on success or 1 on failure (if the indexes are not valid)
   */
  bool exchange(const size_t index1, const size_t index2);

  size_t heap_size() const;

  bool max_heapify(const size_t index);

  bool shallow_copy(struct heap_node * const ary, const size_t ary_len);

  bool build_heap(struct heap_node * const ary, const size_t ary_len);

  bool build_max_heap(struct heap_node * const ary, const size_t ary_len);

  bool heap_sort(struct heap_node *ary, const size_t ary_len);

  size_t last_index(void);

  bool prim_mst(Graph *g, std::list<struct heap_node *> &mst);

  bool contains(void *data);

  struct heap_node * getNode(void *data);

private:
  /**
   * The container for the contents of the heap
   */
//  struct heap_node **nodes;
  std::vector<struct heap_node *> nodes;
  /**
   * The size of the elements container.  Do not explicitly modify.
   */
//  unsigned long container_size;
  /**
   * The index of the last element in the elements container.
   * Do not explicitly modify.
   */
//  long last_index;
};

#endif /* MBOGO_HEAP_H_ */
