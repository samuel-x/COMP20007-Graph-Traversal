/* * * * * * *
 * Module for creating and manipulating singly-linked lists of integers
 *
 * created for COMP20007 Design of Algorithms 2017
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 *
 * edited for COMP2007 Design of Algorithms 2017 Assignment 1
 * by Samuel Xu
 */

#include <stdlib.h>
#include <assert.h>
#include "list.h"

typedef struct node Node;

// a list node points to the next node in the list, and to some data
struct node {
	Node *next;
	int id;
	int seen;
	char* label;
};

// a list points to its first and last nodes, and stores its size (num. nodes)
struct list {
	Node *head;
	Node *tail;
	int size;
};

// helper function to create a new node and return its address
Node *new_node();

// helper function to clear memory of a node (does not free the node's data)
void free_node(Node *node);


/* * * *
 * FUNCTION DEFINITIONS
 */

// create a new list and return a pointer to it
List *new_list() {
	List *list = malloc(sizeof *list);
	assert(list);
	
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;

	return list;
}

// destroy a list and free its memory
void free_list(List *list) {
	assert(list != NULL);
	// free each node
	Node *node = list->head;
	Node *next;
	while (node) {
		next = node->next;
		free_node(node);
		node = next;
	}
	// free the list struct itself
	free(list);
}

// helper function to create a new node and return its address
Node *new_node() {
	Node *node = malloc(sizeof *node);
	assert(node);
	
	return node;
}

// helper function to clear memory of a node
void free_node(Node *node) {
	free(node);
}

// add an element to the back of a list
// this operation is O(1)
void list_add_end(List *list, int data, int seen, char* label) {
	assert(list != NULL);
	
	// we'll need a new list node to store this data
	Node *node = new_node();
	node->id = data;
	node->seen = seen;
	strcpy(node->label, label);
	node->next = NULL; // as the last node, there's no next node

	if(list->size == 0) {
		// if the list was empty, new node is now the first node
		list->head = node;
	} else {
		// otherwise, it goes after the current last node
		list->tail->next = node;
	}
	
	// place this new node at the end of the list
	list->tail = node;
	
	// and keep size updated too
	list->size++;
}

// return the number of elements contained in a list
int list_size(List *list) {
	assert(list != NULL);
	return list->size;
}

// returns whether the list contains no elements (true) or some elements (false)
bool list_is_empty(List *list) {
	assert(list != NULL);
	return (list->size==0);
}