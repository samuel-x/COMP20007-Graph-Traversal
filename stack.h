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

#include <stdbool.h>

// a stack node points to the next node in the stack, and to some data

typedef struct node Node;
struct node {
	int id;
	int visited;
	int distance;
	char* label;
	Node *next;
};

// a stack points to its first and last nodes, and stores its size (num. nodes)
typedef struct stack Stack;
struct stack {
	Node *head;
	Node *tail;
	Node *next_stack;
	int size;
};

// helper function to create a new node and return its address
Node *new_node();

// helper function to clear memory of a node (does not free the node's data)
void free_node(Node *node);

// create a new stack and return a pointer to it
Stack *new_stack();

// destroy a stack and free its memory
void free_stack(Stack *stack);

// add an element to the back of a stack
// this operation is O(1)
void stack_add_end(Stack *stack, int data, int distance, int visited, char* label);

int stack_remove_end(Stack *stack);

// return the number of elements contained in a stack
int stack_size(Stack *stack);

// returns whether the stack contains no elements (true) or some elements (false)
bool stack_is_empty(Stack *stack);