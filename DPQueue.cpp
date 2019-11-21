// FILE: DPQueue.cpp
// IMPLEMENTS: p_queue (see DPQueue.h for documentation.)
//
// INVARIANT for the p_queue class:
//   1. The number of items in the p_queue is stored in the member
//      variable used.
//   2. The items themselves are stored in a dynamic array (partially
//      filled in general) organized to follow the usual heap storage
//      rules.
//      2.1 The member variable heap stores the starting address
//          of the array (i.e., heap is the array's name). Thus,
//          the items in the p_queue are stored in the elements
//          heap[0] through heap[used - 1].
//      2.2 The member variable capacity stores the current size of
//          the dynamic array (i.e., capacit1y is the maximum number
//          of items the array currently can accommodate).
//          NOTE: The size of the dynamic array (thus capacity) can
//                be resized up or down where needed or appropriate
//                by calling resize(...).
// NOTE: Private helper functions are implemented at the bottom of
// this file along with their precondition/postcondition contracts.

#include <cassert>   // provides assert function
#include <iostream>  // provides cin, cout
#include <iomanip>   // provides setw
#include <cmath>     // provides log2
#include "DPQueue.h"

using namespace std;

namespace CS3358_FA2019_A7
{
   // EXTRA MEMBER FUNCTIONS FOR DEBUG PRINTING
   void p_queue::print_tree(const char message[], size_type i) const
   // Pre:  (none)
   // Post: If the message is non-empty, it has first been written to
   //       cout. After that, the portion of the heap with root at
   //       node i has been written to the screen. Each node's data
   //       is indented 4*d, where d is the depth of the node.
   //       NOTE: The default argument for message is the empty string,
   //             and the default argument for i is zero. For example,
   //             to print the entire tree of a p_queue p, with a
   //             message of "The tree:", you can call:
   //                p.print_tree("The tree:");
   //             This call uses the default argument i=0, which prints
   //             the whole tree.
   {
      const char NO_MESSAGE[] = "";
      size_type depth;

      if (message[0] != '\0')
         cout << message << endl;

      if (i >= used)
         cout << "(EMPTY)" << endl;
      else
      {
         depth = size_type( log( double(i+1) ) / log(2.0) + 0.1 );
         if (2*i + 2 < used)
            print_tree(NO_MESSAGE, 2*i + 2);
         cout << setw(depth*3) << "";
         cout << heap[i].data;
         cout << '(' << heap[i].priority << ')' << endl;
         if (2*i + 1 < used)
            print_tree(NO_MESSAGE, 2*i + 1);
      }
   }

   void p_queue::print_array(const char message[]) const
   // Pre:  (none)
   // Post: If the message is non-empty, it has first been written to
   //       cout. After that, the contents of the array representing
   //       the current heap has been written to cout in one line with
   //       values separated one from another with a space.
   //       NOTE: The default argument for message is the empty string.
   {
      if (message[0] != '\0')
         cout << message << endl;

      if (used == 0)
         cout << "(EMPTY)" << endl;
      else
         for (size_type i = 0; i < used; i++)
            cout << heap[i].data << ' ';
   }

   // CONSTRUCTORS AND DESTRUCTOR

   //Name: p_queue(size_type initial_capacity = DEFAULT_CAPACITY)
   //Summary: Constructor - initializes p-queue object 
   //Pre Conditons:  initial_capacity > 0
   //Post Conditions: The p_queue has been initialized to an empty p_queue.
   //    The push function will work efficiently (without allocating
   //    new memory) until this capacity is reached.
   //   Note: If Pre is not met, initial_capacity will be adjusted to
   //    DEFAULT_CAPACITY. I.e., when creating a p_queue object,
   //    client can override initial_capacity with something deemed
   //    more appropriate than DEFAULT_CAPACITY; but if (in doing so)
   //    client mis-specifies 0 (NOTE: size_type is unsigned, thus
   //    can't be negative) as the overriding size, DEFAULT_CAPACITY
   //    remains as the value to be used for initial_capacity (this
   //    is to ensure no attempt is made at allocating memory that's
   //    0 in amount).
   //Parameter(s):
   //    inital_capacity - capacity of array at initialization 
   //Returns: (none)
   p_queue::p_queue(size_type initial_capacity) : capacity (initial_capacity), used(0) 
   {
      if (capacity < 1)
         capacity = DEFAULT_CAPACITY;
      heap = new ItemType[capacity]; 
   }

   //Name: p_queue::p_queue(const p_queue& src) 
   //Summary: Copy Constructor - initilizes invoking using src
   //Pre-Conditions: (none)
   //Post-Conditions:(none)
   //Parameter(s): 
   //    src - address of object to be copied from 
   //Returns: (none)
   p_queue::p_queue(const p_queue& src) : capacity(src.capacity), used(src.used)
   {
      heap = new ItemType[capacity]; 
      for(size_type i = 0; i < used; i++)
         heap[i] = src.heap[i]; 
   }

   //Name: p_queue::~p_queue()  
   //Summary: Destructor - releases allocated memory at end of object lifetime
   //Pre-Conditions: (none)
   //Post-Conditions: (none)
   //Parameter(s): (none)
   //Returns: (none)
   p_queue::~p_queue()
   {
      delete [] heap; 
   }

   // MODIFICATION MEMBER FUNCTIONS (MUTATORS)

   //Name: p_queue& p_queue::operator=(const p_queue& rhs)  
   //Summary: Assignment Operator - assigns rhs data to invoking object
   //Pre-Conditions: (none)
   //Post-Conditions: (none)
   //Parameter(s): 
   //    rhs - address of object to be copied from 
   //Returns: (none)
   p_queue& p_queue::operator=(const p_queue& rhs)
   {
      if (this != &rhs)
      {
         ItemType* newHeap = new ItemType[rhs.capacity];
         for(size_type i = 0; i < rhs.used; i++)
            newHeap[i] = rhs.heap[i];
         delete [] heap;
         heap = newHeap;
         capacity = rhs.capacity; 
         used = rhs.used; 
      }  
      return *this;
   }

   //Name: void push(const value_type& entry, size_type priority)
   //Summary: pushes on a new item to the priority queue 
   //Pre Conditons:  (none)
   //Post Conditions: A new copy of item with the specified data and priority
   //    has been added to the p_queue.
   //Parameter(s): 
   //    entry - data fieled of new Item to be queued 
   //    priority - priority of new Item to be queued
   //Returns: (none)
   void p_queue::push(const value_type& entry, size_type priority)
   {
      if (used == capacity)
         resize((1.5 * capacity) + 1);
      heap[used].data = entry;
      heap[used].priority = priority; 
      used++; 
      for (size_type i = (used - 1); i > 0; i = parent_index(i))
      {
         if (heap[i].priority > parent_priority(i))
         {
            swap_with_parent(i);
         }
      }
   }

   //Name: void p_queue::pop()
   //Summary: pops off the highest priority item  
   //Pre Conditons: size() > 0
   //Post Conditions:  The highest priority item has been removed from the
   //    p_queue. (If several items have the equal priority,
   //    then the implementation may decide which one to remove.)
   //Parameter(s): (none)
   //Returns: (none)
   void p_queue::pop()
   { 
      assert(!empty()); 
      if (used > 1)
      {
         heap[0] = heap[used - 1];
         used--;
         if(!empty())
         {
            for (size_type i = 0; i < used; i++)
            {
               if(!is_leaf(i))
               { 
                  if (heap[i].priority < big_child_priority(i))
                     swap_with_parent(big_child_index(i)); 
               }
            }
         }
      }
      else
         used--;
   }

   // CONSTANT MEMBER FUNCTIONS (ACCESSORS)

   //Name: size_type p_queue::size() const 
   //Summary: Accessor - gets size of invoking IntSet
   //Pre-Conditions: (none)
   //Post-Conditions:The return value is the total number of items in the
   //    p_queue.
   //Parameter(s): (none)
   //Returns: size of data array
   p_queue::size_type p_queue::size() const
   {
      return used; 
   }

   //Name: bool p_queue::empty() const 
   //Summary: Accessor - checks to see if the invoking p_queue is empty
   //Pre-Conditions: (none)
   //Post-Conditions: The return value is true if the p_queue is empty,
   //    otherwise false.
   //Parameter(s): (none)
   //Returns: true value if array is empty, false otherwise
   bool p_queue::empty() const
   {
      if (used == 0)
         return true;
      else
         return false; 
   }

   //Name:  p_queue::value_type p_queue::front() const
   //Summary: Accessor - returns front of the heap 
   //Pre-Conditions: size() > 0
   //Post-Conditions: The return value is the data of the highest priority
   //    item in the p_queue, but the p_queue is unchanged.
   //    (If several items have equal priority, then the
   //    implementation may decide which one to return.)
   //Parameter(s): (none)
   //Returns: the highest priority item's data value 
   p_queue::value_type p_queue::front() const
   {
      assert(!empty());   
      return heap[0].data; 
   } 

   // PRIVATE HELPER FUNCTIONS

   //Name: void p_queue::resize(size_type new_capacity)
   //Summary: Helper - resizes the dynamic array based on new_capacity
   //Pre-Conditions: (none)
   //Post-Conditions: The size of the dynamic array pointed to by heap (thus
   //    the capacity of the p_queue) has been resized up or down
   //    to new_capacity, but never less than used (to prevent
   //    loss of existing data).
   //    NOTE: All existing items in the p_queue are preserved and
   //    used remains unchanged.
   //Parameter(s): 
   //    new_capacity - the new capacity of the invoking object
   //Returns: (none)
   void p_queue::resize(size_type new_capacity)
   {
      if (new_capacity < used)
         new_capacity = used;
      if (used < 0)
         new_capacity = DEFAULT_CAPACITY;
      capacity = new_capacity; 
      ItemType* newHeap = new ItemType[capacity];
      for (size_type i = 0; i < used; i++)
         newHeap[i] = heap[i];
      delete [] heap;
      heap = newHeap;  
   }

   //Name: bool p_queue::is_leaf(size_type i) const
   //Summary: Helper - returns true if heap[i] is a leaf 
   //Pre: (i < used)
   //Post: If the item at heap[i] has no children, true has been
   //    returned, otherwise false has been returned.
   //Parameter(s): 
   //    i - the index of the item we want to evaluate at heap[i]
   //Returns: true if heap[i] is a leaf, false otherwise 
   bool p_queue::is_leaf(size_type i) const
   {
      assert(i < used);
      if (((2 * i) + 1) == used && ((2 * i) + 2) > used)
         return true;
      else 
         return false; 
   }

   //Name:  p_queue::size_type p_queue::parent_index(size_type i) const
   //Summary: Helper - returns the index of the parent 
   //Pre:  (i > 0) && (i < used)
   //Post: The index of "the parent of the item at heap[i]" has
   //    been returned.
   //Parameter(s): 
   //    i - the index of the item whose parent we want
   //Returns: index of the parent 
   p_queue::size_type p_queue::parent_index(size_type i) const
   {
      assert((i > 0) && (i < used));
      return ((i - 1)/2); 
   }

   //Name: p_queue::size_type p_queue::parent_priority(size_type i) const
   //Summary: Helper - returns the priority of the parent at heap[i]
   //Pre: (i > 0) && (i < used)
   //Post: The priority of "the parent of the item at heap[i]" has
   //    been returned.
   //Parameter(s): 
   //    i - the index of the item whose parent we want
   //Returns: priority of the parent 
   p_queue::size_type p_queue::parent_priority(size_type i) const
   {
      assert((i > 0) && (i < used));
      return (heap[(i - 1)/2].priority);  
   }

   //Name: p_queue::size_type p_queue::big_child_index(size_type i) const
   //Summary: Helper - returns the index of the bigger child at heap[i]
   //Pre: is_leaf(i) returns false
   //Post: The index of "the bigger child of the item at heap[i]"
   //    has been returned.
   //    (The bigger child is the one whose priority is no smaller
   //    than that of the other child, if there is one.)
   //Parameter(s): 
   //    i - the index of the item whose bigger child we want
   //Returns: index of the bigger child 
   p_queue::size_type p_queue::big_child_index(size_type i) const
   {
      assert(!is_leaf(i)); 
      if (((2 * i) + 2) < used) //there is a left and right child
      {
         if (heap[(2 * i) + 1].priority > heap[(2 * i) + 2].priority)
            return ((2 * i) + 1);
               else
         return ((2 * i) + 2);
      }
      else //there is only a left child 
         return ((2 * i) + 1); 
   }

   //Name: p_queue::size_type p_queue::big_child_priority(size_type i) const
   //Summary: Helper - returns the priority of the bigger child at heap[i]
   //Pre: is_leaf(i) returns false
   //Post: The priority of "the bigger child of the item at heap[i]"
   //    has been returned.
   //    (The bigger child is the one whose priority is no smaller
   //    than that of the other child, if there is one.)
   //Parameter(s): 
   //    i - the index of the item whose bigger child we want
   //Returns: priority of the bigger child 
   p_queue::size_type p_queue::big_child_priority(size_type i) const
   {
      assert(!is_leaf(i)); 
      if (((2 * i) + 2) < used) //there is a left and right child
      {
         if (heap[(2 * i) + 1].priority > heap[(2 * i) + 2].priority)
            return heap[(2 * i) + 1].priority;
         else
            return heap[(2 * i) + 2].priority;

      }
      else //there is only a left child
         return heap[(2 * i) + 1].priority;
      
   }

   //Name:    void p_queue::swap_with_parent(size_type i)
   //Summary: Helper - swaps the given heap[i] with the parent 
   //Pre: (i > 0) && (i < used)
   //Post: The item at heap[i] has been swapped with its parent.
   //Parameter(s): 
   //    i - the index of the heap[i] to be swapped with it's parent
   //Returns: (none) 
   void p_queue::swap_with_parent(size_type i)
   {
      assert((i > 0) && (i < used));
      ItemType copy;
      copy = heap[i]; 
      heap[i] = heap[parent_index(i)];
      heap[parent_index(i)] = copy;                                      
   }
}