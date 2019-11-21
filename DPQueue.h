// FILE: DPQueue.h
// CLASS PROVIDED: p_queue (priority queue ADT)
//
// TYPEDEFS and MEMBER CONSTANTS for the p_queue class:
//   typedef _____ value_type
//     p_queue::value_type is the data type of the items in
//     the p_queue. It may be any of the C++ built-in types
//     (int, char, etc.), or a class with a default constructor, a
//     copy constructor, an assignment operator, and a less-than
//     operator forming a strict weak ordering.
//
//   typedef _____ size_type
//     p_queue::size_type is the data type considered best-suited
//     for any variable meant for counting and sizing (as well as
//     array-indexing) purposes; e.g.: it is the data type for a
//     variable representing how many items are in the p_queue.
//     It is also the data type of the priority associated with
//     each item in the p_queue
//
//   static const size_type DEFAULT_CAPACITY = _____
//    p_queue::DEFAULT_CAPACITY is the default initial capacity of a
//    p_queue that is created by the default constructor.

// VALUE SEMANTICS for the p_queue class:
//   Assignments and the copy constructor may be used with p_queue
//   objects.

#ifndef D_P_QUEUE_H
#define D_P_QUEUE_H

#include <cstdlib> // provides size_t

namespace CS3358_FA2019_A7
{
   class p_queue
   {
   public:
      // TYPEDEFS and MEMBER CONSTANTS
      typedef int value_type;
      typedef size_t size_type;
      static const size_type DEFAULT_CAPACITY = 1;
      // CONSTRUCTORS AND DESTRUCTOR
      p_queue(size_type initial_capacity = DEFAULT_CAPACITY);
      p_queue(const p_queue& src);
      ~p_queue();
      // MODIFICATION MEMBER FUNCTIONS
      p_queue& operator=(const p_queue& rhs);
      void push(const value_type& entry, size_type priority);
      void pop();
      // CONSTANT MEMBER FUNCTIONS
      size_type size() const;
      bool empty() const;
      value_type front() const;
      // EXTRA CONSTANT MEMBER FUNCTION FOR DEBUG PRINTING
      void print_tree(const char message[] = "", size_type i = 0) const;
      void print_array(const char message[] = "") const;
   private:
      // STRUCT to store information about one item in the p_queue
      struct ItemType{
         value_type data;
         size_type priority;
      };
      // PRIVATE MEMBER VARIABLES
      ItemType *heap;
      size_type capacity;
      size_type used;
      // HELPER FUNCTIONS
      void resize(size_type new_capacity);
      bool is_leaf(size_type i) const;
      size_type parent_index(size_type i) const;
      size_type parent_priority(size_type i) const;
      size_type big_child_index(size_type i) const;
      size_type big_child_priority(size_type i) const;
      void swap_with_parent(size_type i);
   };
}
#endif