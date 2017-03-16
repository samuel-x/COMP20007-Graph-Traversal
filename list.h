/* * * * * * *
 * Module for creating and manipulating singly-linked lists of integers
 *
 * created for COMP20007 Design of Algorithms 2017
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 */

#include <stdbool.h>

typedef struct list List;

// create a new list and return a pointer to it
List *new_list();

// destroy a list and free its memory
void free_list(List *list);

// add an element to the back of a list
// this operation is O(1)
void list_add_end(List *list, int data, int seen, char* label);

// return the number of elements contained in a list
int list_size(List *list);

// returns whether the list contains no elements (true) or some elements (false)
bool list_is_empty(List *list);