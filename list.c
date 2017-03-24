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
#include <string.h>
#include <assert.h>
#include "list.h"





// a list points to its first and last nodes, and stores its size (num. nodes)





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
void list_add_end(List *list, int data, int distance, int visited, char* label) {
	assert(list != NULL);
	
	// we'll need a new list node to store this data
	Node *node = new_node();
	node->id = data;
	node->visited = visited;
	node->distance = distance;
	node->label = malloc(sizeof(label));
	strcpy(node->label, label);
	//label[sizeof(label)+1] = '\0';
	node->next = NULL; // as the last node, there's no next node

	if(list->size == 0) {
		// if the list was empty, new node is now the first node
		list->head = node;
		list->head->prev = NULL;
	} else {
		// otherwise, it goes after the current last node
		list->tail->next = node;
		node->prev = list->tail;
	}
	
	// place this new node at the end of the list
	list->tail = node;
	
	// and keep size updated too
	list->size++;
}

// remove and return the final data element in a list
// this operation is O(n), where n is the number of elements in the list
// error if the list is empty (so first ensure list_size() > 0)
int list_remove_end(List *list) {
	assert(list != NULL);
	assert(list->size > 0);
	
	// we'll need to save the data to return it
	Node *end_node = list->tail;
	int id = end_node->id;
	
	// then replace the tail with the second-last node (may be null)
	// (to find this replacement, we'll need to walk the list --- the O(n) bit
	Node *node = list->head;
	Node *prev = NULL;
	while (node->next) {
		prev = node;
		node = node->next;
	}
	list->tail = prev;
	
	if(list->size == 1) {
		// if we're removing the last node, the head also needs clearing
		list->head = NULL;
	} else {
		// otherwise, the second-last node needs to drop the removed last node
		prev->next = NULL;
	}

	// the list just got one element shorter
	list->size--;

	// we're finished with the list node holding this data
	free_node(end_node);

	// done!
	return id;
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