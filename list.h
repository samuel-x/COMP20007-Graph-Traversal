/* * * * * * *
 * Module for creating and manipulating singly-linked lists of integers
 *
 * created for COMP20007 Design of Algorithms 2017
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 */

#include <stdbool.h>

// a list node points to the next node in the list, and to some data

typedef struct node Node;
struct node {
	int id;
	int visited;
	int distance;
	char* label;
	Node *next;
	Node *prev;
};

typedef struct list List;
struct list {
	Node *head;
	Node *tail;
	Node *next_list;
	int size;
};

// helper function to create a new node and return its address
Node *new_node();

// helper function to clear memory of a node (does not free the node's data)
void free_node(Node *node);

// create a new list and return a pointer to it
List *new_list();

// destroy a list and free its memory
void free_list(List *list);

// add an element to the back of a list
// this operation is O(1)
void list_add_end(List *list, int data, int distance, int visited, char* label);

int list_remove_end(List *list);

// return the number of elements contained in a list
int list_size(List *list);

// returns whether the list contains no elements (true) or some elements (false)
bool list_is_empty(List *list);