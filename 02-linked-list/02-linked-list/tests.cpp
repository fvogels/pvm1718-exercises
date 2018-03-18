#define CATCH_CONFIG_MAIN
#include "Catch.h"
#include "linked-list.h"


/*

See https://en.wikipedia.org/wiki/Linked_list if you have no idea what linked lists are.

Write a LinkedList class whose objects represent a node in a linked list.
Each node has two fields:
  * value: an integer
  * next: a pointer to the next node in the linked list

The last node has next = nullptr. By chaining nodes together, it is possible to construct a list of arbitrary size.

Reverse engineer the tests to deduce what members LinkedList should have.

*/