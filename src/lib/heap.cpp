/**
 * @author Mike Bogochow
 * @version 3.2.0, Dec 7, 2015
 *
 * @file heap.cpp
 *
 * Array based heap.
 */

#include "heap.h"

//#include <stdlib.h>
//#include <errno.h>
//#include <string.h>
//#include <stdio.h>
#include <assert.h>
#include <algorithm>

#if defined(HEAP_DEBUG) && HEAP_DEBUG > 0
void print_heap(struct heap * const heap)
{
  int i;

  fprintf(stderr, "==== Heap Print ====\n");
  fprintf(stderr, "Heap Size: %lu\n", heap->container_size);
  fprintf(stderr, "Heap Last Index: %li\n", heap->last_index);

  fprintf(stderr, "Heap elements:\n");
  for (i = 0; i <= heap->last_index; i++)
    fprintf(stderr, "el[%i]: %f\n", i, nodes.data()[i]->value);

  fprintf(stderr, "\n");
}
#endif

Heap::Heap(void)
{
  resize();
}

Heap::~Heap(void)
{

}

size_t
Heap::last_index(void)
{
  return nodes.size() - 1;
}

bool
Heap::resize(void)
{
  bool ret = true;
  size_t new_heap_length;

  if (nodes.capacity() == 0)
    new_heap_length = INIT_HEAP_SIZE;
  else
    new_heap_length = nodes.capacity() * 2;

#if DBG_HEAP_LVL(1)
  fprintf(stderr, "Resizing array from size %lu to size %lu\n",
      heap->container_size, new_heap_length);
#endif

  nodes.reserve(new_heap_length);

  return ret;
} /* resize */

bool
Heap::exchange(const size_t index1, const size_t index2)
{
  bool ret = true;

#if DBG_HEAP_LVL(1)
  fprintf(stderr, "EXCHNG> Exchanging index %i (%f) with index %i (%f)\n",
      index1, nodes.data()[index1]->value, index2, nodes.data()[index2]->value);
#endif

  if (is_valid_index(nodes.capacity(), index1)
      && is_valid_index(nodes.capacity(), index2))
  {
    std::iter_swap(nodes.begin() + index1, nodes.begin() + index2);
  }

  else
  {
#ifdef HEAP_DEBUG
    fprintf(stderr, "Invalid indices passed to the heap exchange function:\n");
    fprintf(stderr, "\tindex1: %i\n", index1);
    fprintf(stderr, "\tindex2: %i\n", index2);
    fprintf(stderr, "\tcapacity: %lu\n", heap->container_size);
#endif
    ret = false;
  }

  return ret;
} /* exchange */

void
Heap::clean_heap()
{
//  free(nodes);
//  heap->last_index = -1;
} /* clean_heap */

bool
Heap::is_smaller(const size_t index1, const size_t index2) const
{
  return nodes.data()[index1]->value < nodes.data()[index2]->value;
} /* is_before */

bool
Heap::get_lesser_index(const size_t index1, const size_t index2) const
{
  return is_smaller(index1, index2) ? index1 : index2;
} /* get_lesser_index */

bool
Heap::is_larger(const size_t index1, const size_t index2) const
{
  return nodes.data()[index1]->value > nodes.data()[index2]->value;
} /* is_after */

size_t
Heap::heap_size() const
{
//  assert(heap->last_index >= -1);
  return nodes.size();
} /* heap_size */

bool
Heap::max_heapify(const size_t index)
{
  bool ret = true;
  size_t l = get_left_child_index(index);
  size_t r = get_right_child_index(index);
  size_t largest;

  if (l <= heap_size() && is_larger(l, index))
    largest = l;

  else
    largest = index;

  if (r <= heap_size() && is_larger(r, largest))
    largest = r;

  if (largest != index)
  {
    if ((ret = exchange(index, largest)))
      ret = max_heapify(largest);
  }

  return ret;
} /* max_heapify */

bool
Heap::shallow_copy(struct heap_node * const ary, const size_t ary_len)
{
  bool ret = true;

  if (ary != nullptr)
  {
    for (size_t i = 0; i < ary_len; i++)
      nodes.push_back(&(ary[i]));
  }

  return ret;
}

/*static void direct_copy(struct heap_node ** const nodes, struct heap_node *const ary)
 {
 assert(nodes);
 *nodes = ary;
 }*/

/*static unsigned int deep_copy(struct heap_node **nodes,
 struct heap_node * const ary, const int ary_len)
 {
 unsigned int ret;

 assert(nodes);
 assert(ary);

 if ((ret = alloc_nodes(nodes, ary_len)) == EXIT_SUCCESS)
 {
 memcpy(*nodes, ary, ary_len * sizeof(struct heap_node));
 }

 return ret;
 }*/

bool
Heap::build_heap(struct heap_node * const ary, const size_t ary_len)
{
  bool ret = false;

//  nodes = nullptr;

  /* Put the values into the heap */
  if ((ret = shallow_copy(ary, ary_len)))
  {
//    nodes.reserve(ary_len);
//    heap->last_index = ary_len - 1;

#if DBG_HEAP_LVL(2)
    fprintf(stderr, "Heap before heapify:\n");
    print_heap(heap);
#endif
  }

  return ret;
} /* build_heap */

bool
Heap::build_max_heap(struct heap_node * const ary, const size_t ary_len)
{
  bool ret;

  if ((ret = build_heap(ary, ary_len)))
  {
    /* Cast to int truncates fraction (i.e. floor) */
    const size_t half_len = (size_t)((double)ary_len / 2.0);

    for (int i = half_len; i >= 0; i--)
    {
      if ((ret = max_heapify(i)) == false)
        break;
    }
  }

  return ret;
} /* build_max_heap */

bool
Heap::heap_sort(struct heap_node *ary, const size_t ary_len)
{
  bool ret = false;

  if ((ret = build_max_heap(ary, ary_len)))
  {
    for (int i = ary_len - 1; i > 0; i--)
    {
      if ((ret = exchange(0, i)))
      {
        nodes.pop_back();
        max_heapify(0);
      }

      else
        break;
    }
  }

  return ret;
} /* heap_sort */

bool
Heap::build_min_heap(struct heap_node* const ary, const size_t ary_len)
{
  bool ret;

  if ((ret = build_heap(ary, ary_len)))
  {
    int i;
    /* Cast to int truncates fraction (i.e. floor) */
    const size_t half_len = (size_t)((double)ary_len / 2.0);

    for (i = half_len; i >= 0; i--)
    {
      if ((ret = push_down(i)) == false)
        break;
    }
  }

  return ret;
} /* build_min_heap */

bool
Heap::pull_up(const size_t index)
{
  size_t parent_index = get_parent_index(index);
  bool ret = true;

  if (is_smaller(index, parent_index))
  {
    if ((ret = exchange(index, parent_index)))
      ret = pull_up(parent_index);
  }

  return ret;
} /* pull_up */

bool
Heap::push_down(const size_t index)
{
  bool ret = true;
  size_t left_child_index = get_left_child_index(index);
  size_t right_child_index = get_right_child_index(index);

  size_t smallest_child = index;
  size_t smallest;

  if (left_child_index <= last_index())
    smallest_child = left_child_index;

  if (right_child_index <= last_index()
      && is_smaller(right_child_index, left_child_index))
  {
    smallest_child = right_child_index;
  }

  /* Must be called with this parameter order or there will be an unnecessary
   * exchange if they are equal */
  smallest = get_lesser_index(smallest_child, index);

#if DBG_HEAP_LVL(1)
#if DBG_HEAP_LVL(2)
  fprintf(stderr, "PSHDWN> ==== START ====\n");
#endif
  fprintf(stderr, "PSHDWN> Pushing down element %i (%f)\n", index,
      nodes.data()[index]->value);
#if DBG_HEAP_LVL(2)
  fprintf(stderr, "PSHDWN> Left Child Index: %i (%f)\n", left_child_index,
      left_child_index <= heap->last_index ?
          nodes.data()[left_child_index]->value : -1);
  fprintf(stderr, "PSHDWN> Right Child Index: %i (%f)\n", right_child_index,
      right_child_index <= heap->last_index ?
          nodes.data()[right_child_index]->value : -1);
  fprintf(stderr, "PSHDWN> Smallest Child Index: %i (%f)\n", smallest_child,
      nodes.data()[smallest_child]->value);
  fprintf(stderr, "PSHDWN> Smallest: %i (%f)\n", smallest,
      nodes.data()[smallest]->value);
#endif
#endif

  /* Exchange with the smallest */
  if (smallest != index)
  {
    if ((ret = exchange(index, smallest)))
    {
#if DBG_HEAP_LVL(2)
      fprintf(stderr, "heap.c:push_down: Heap after exchange, before "
          "push_down:\n");
      print_heap(heap);
#endif
      ret = push_down(smallest);
    }
  }

  return ret;
} /* push_down */

bool
Heap::insert(struct heap_node * const element)
{
  bool ret = true;

#if DBG_HEAP_LVL(1)
  fprintf(stderr, "INSRT> Inserting %f into heap\n", element->value);
  fprintf(stderr, "INSRT> heap->last_index: %li\n", heap->last_index);
  fprintf(stderr, "INSRT> heap->size: %lu\n", heap->container_size);
#endif

  /* Resize the heap if it is full */
  if (last_index() == nodes.capacity() - 1)
    ret = resize();

  if (ret)
  {
    /* Add the element to the end of the heap, increasing size and last index */
    nodes.push_back(element);
//    nodes.data()[++heap->last_index] = element;
//    element->index = heap->last_index;

    /* Pull up to reestablish the invariant */
    ret = pull_up(last_index());
  }

  return ret;
} /* insert */

bool
Heap::extract_min(struct heap_node **min)
{
  bool ret = false;

  *min = nodes.data()[0];

#if DBG_HEAP_LVL(1)
  fprintf(stderr, "EXTRCT> Extracting minimum value %f from heap\n",
      (*min)->value);
#endif

  if (last_index() >= 0)
  {
//    /* Overwrite first element with last.
//     * Decrease the last index to effectively remove the last element */
    std::iter_swap(nodes.begin(), nodes.end());
    nodes.pop_back();

//    nodes.data()[0] = nodes.data()[heap->last_index--];
//    nodes.data()[0]->index = 0;
//    (*min)->index = -1;

    /* Push down to reestablish invariant */
    ret = push_down(0);

#ifdef HEAP_DEBUG
    if (ret == EXIT_FAILURE)
      fprintf(stderr, "Push down failed while attempting to extract %f\n",
          (*min)->value);
#endif
  }

  return ret;
} /* extract_min */
