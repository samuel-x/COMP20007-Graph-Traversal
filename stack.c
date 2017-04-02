/* * * * * * *
 * Module for creating and manipulating singly-linked stacks of integers
 *
 * created for COMP20007 Design of Algorithms 2017
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 *
* Sourced from Week 3 Design of Algorithms Tutorial/Lab 2017
 * edited for COMP2007 Design of Algorithms 2017 Assignment 1
 * by Samuel Xu <samuelx@student.unimelb.edu.au
 * * * * * * */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "stack.h"

// helper function to create a new node and return its address
Node *new_node();

// helper function to clear memory of a node (does not free the node's data)
void free_node(Node *node);

/* * * *
 * FUNCTION DEFINITIONS
 */

// create a new stack and return a pointer to it
Stack *new_stack() {
	Stack *stack = malloc(sizeof *stack);
	assert(stack);
	
	stack->head = NULL;
	stack->tail = NULL;
	stack->size = 0;

	return stack;
}

// destroy a stack and free its memory
void free_stack(Stack *stack) {
	assert(stack != NULL);
	// free each node
	Node *node = stack->head;
	Node *next;
	while (node) {
		next = node->next;
		free_node(node);
		node = next;
	}
	// free the stack struct itself
	free(stack);
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


// add an element to the back of a stack
// this operation is O(1)
void stack_add_end(Stack *stack, int data, int distance, int visited, char* label) {
	assert(stack != NULL);
	
	// we'll need a new stack node to store this data
	Node *node = new_node();

	// assign data
	node->id = data;
	node->visited = visited;
	node->distance = distance;
	node->label = malloc(sizeof(label));
	strcpy(node->label, label);
	node->next = NULL; // as the last node, there's no next node

	if(stack->size == 0) {
		// if the stack was empty, new node is now the first node
		stack->head = node;
	} else {
		// otherwise, it goes after the current last node
		stack->tail->next = node;
	}
	
	// place this new node at the end of the stack
	stack->tail = node;
	
	// and keep size updated too
	stack->size++;
}

// remove and return the final data element in a stack
// this operation is O(n), where n is the number of elements in the stack
// error if the stack is empty (so first ensure stack_size() > 0)
int stack_remove_end(Stack *stack) {
	assert(stack != NULL);
	assert(stack->size > 0);
	
	// we'll need to save the data to return it
	Node *end_node = stack->tail;
	int id = end_node->id;
	
	// then replace the tail with the second-last node (may be null)
	// (to find this replacement, we'll need to walk the stack --- the O(n) bit
	Node *node = stack->head;
	Node *prev = NULL;
	while (node->next) {
		prev = node;
		node = node->next;
	}
	stack->tail = prev;
	
	if(stack->size == 1) {
		// if we're removing the last node, the head also needs clearing
		stack->head = NULL;
	} else {
		// otherwise, the second-last node needs to drop the removed last node
		prev->next = NULL;
	}

	// the stack just got one element shorter
	stack->size--;

	// we're finished with the stack node holding this data
	free_node(end_node);

	// done!
	return id;
}


// return the number of elements contained in a stack
int stack_size(Stack *stack) {
	assert(stack != NULL);
	return stack->size;
}

// returns whether the stack contains no elements (true) or some elements (false)
bool stack_is_empty(Stack *stack) {
	assert(stack != NULL);
	return (stack->size==0);
}