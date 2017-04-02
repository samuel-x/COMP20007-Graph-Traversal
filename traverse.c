// Design of Algorithms Assignment 1
// by Samuel Xu
// Date Finished (ver 2.0) | 24/03/17
/*
This program has several functions involving the traversal of a 
non-directional, singular graph.
These functions include,
	1. Traversal using Depth First Search
	2. Traversal using Breadth First Search
	3. Traversal using Depth First Search (and recording the 
			cumulative distance of each node)
	4. Finding all possible path between two Vertexes (via recursive DFS)
	5. Finding the shortest possible path between two Vertexes (via recursive DFS)
*/
/*
"Style" Guide to self 2.1 (to keep things consistent):
-	Operations with variables should have spaces in between.
	This is to prevent issues with pointer operations and the divisor operator
	becoming a comment.
	E.g.
		int *i=0;
		vs
		int *i = *foo / *fishsticks; <-- to prevent this becoming a comment

-	If a bracket goes past 80 characters, newline where appropriate and tab
	the arguments so that it matches up to the bracket / previous argument.

-	All functions should open with { on the same line, and terminate with } 
	in the sametab index.

-	Control structures should have a space between the name and bracket.

-	Function arguments should be separated by 1 space.

-	Typedef should be declared in the same way that functions are declared
	(i.e. starting bracket on the same line and ending bracket with a newline)

-	Conditions with && and || must be seperated by a space. In for loops, put
	a space between each semicolon. 

-	Comparisons should have no spaces in between operators, unless in the case
	of very long (~10+ characters) conditions for the sake of structs.

-	When using malloc or realloc, make sure the multiplicative factor goes 
	before sizeof or whatever is used to determine the size. This is to prevent
	accidental pointer dereferencing.

-	Declare ints first, then float, then char, then other structs / data types.

-	Character limit is at 100
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "traverse.h"
#include "stack.h"

#define FALSE 0
#define TRUE 1
#define NO_ID -1
#define NO_DIST -2
//#define INT_MAX 99999

int check_stack(Stack *stack, Edge *ptr);
void explore_dfs(Graph* graph, Stack *stack, Edge *ptr, int *added_id, int *branch);
void explore_bfs(Graph* graph, Stack *stack, Edge *ptr);
void explore_dfs_all(Graph *graph, Stack *stack, Stack *path, Vertex *current_pos, int destination_id, 
						int one_path, int *exit_flag);
void explore_dfs_shortest(Graph *graph, Stack *stack, Stack *shortest_path, Vertex *current_pos, 
							int destination_id, int *shortest, int prev_weight);
void print_oneline(Stack *stack, int distance);
void print_stack(Stack *stack, int detail);
int check_path(Stack **stack, Stack *path, Edge *ptr, int size);
int calculate_dist(Stack *stack);

//PART 1
void print_dfs(Graph* graph, int source_id) {
	int added_id = NO_ID;
	int branch = 0;
	int exit_flag = 0;
	int i;
	int limit = graph->n;
	// Create a new stack to store towns travelled to
	Stack *stack = new_stack();
	Vertex *current_pos = graph->vertices[source_id];
	Edge *ptr = current_pos->first_edge;
	Node *nodeptr;

	// Add the first vertex to stack (source location)
	stack_add_end(stack, ptr->u, FALSE, TRUE, graph->vertices[ptr->u]->label);
	nodeptr = stack->head;

	// While graph has not fully been traversed, explore until it has
	while (exit_flag != TRUE) {
		// Explore function which checks which path to follow. If no path is found 
		// (as in all edges have been travelled to, make added_id NO_ID)
		explore_dfs(graph, stack, ptr, &added_id, &branch);
		// If no path was found, then backtrack until new path is found.
		if (added_id == NO_ID) {
			nodeptr = stack->head;
			for (i = 0; i<((stack->size-branch)-1); i++) {
				nodeptr = nodeptr->next;
			}
			added_id = nodeptr->id;
			branch = 0;
		}	
		// Move pointer to next vertex.
		current_pos = graph->vertices[added_id];
		ptr = current_pos->first_edge;
		
		nodeptr = nodeptr->next;
		added_id = NO_ID;
		branch++;
		
		// If the same amount of vertexes as in the graph have been travelled to, 
		// then exit successfully.
		if (stack->size>=limit || branch>=limit) {
			exit_flag = TRUE;
		}
	}
	// print the full stack (with no distance measures)
	print_stack(stack, FALSE);
	// Free the stack.
	free_stack(stack);
}

// Helper function for DFS part 1.
void explore_dfs(Graph* graph, Stack *stack, Edge *ptr, int *added_id, int *branch) {
	// check all edges
	while (ptr != NULL) {
		// if the edge is in the stack,
		if (check_stack(stack, ptr) == TRUE) {
			// check if there is currently already a vertex found, if so, then reset the 
			// backtrack counter
			// (meaning you can revert to this vertex if you end up at a dead end)
			if (*added_id != NO_ID) {
				*branch = 0;
				break;
			}
			// Add vertex to stack if not currently in stack
			stack_add_end(stack, ptr->v, ptr->weight, FALSE, graph->vertices[ptr->v]->label);
			*added_id = ptr->v;
		}
		else {
			// If edge is in the stack, then just go to the next edge.
			ptr = ptr->next_edge;
		}
	}
}

// PART 2
void print_bfs(Graph* graph, int source_id) {
	int exit_flag = 0;
	int limit = graph->maxn;

	// Create a new stack to store towns travelled to
	Stack *stack = new_stack();
	Vertex *current_pos = graph->vertices[source_id];
	Edge *ptr = current_pos->first_edge;
	Node *nodeptr;

	// Add the first vertex to stack (source location).
	stack_add_end(stack, ptr->u, FALSE, TRUE, graph->vertices[ptr->u]->label);
	nodeptr = stack->head;	

	while (exit_flag != TRUE) {
		// Explore function which adds all the valid surrounding edges to a stack.
		explore_bfs(graph, stack, ptr);
		// The pointer in the stack keeps track of the 'depth' you are in. If a edge has been seen,
		// but not visited, then move to that vertex in the next iteration.
		if (nodeptr->visited == 0) {
			current_pos = graph->vertices[nodeptr->id];
			nodeptr->visited = TRUE;
			nodeptr = nodeptr->next;
		}
		else {
			nodeptr = nodeptr->next;
		}
		// Move pointer to first_edge of the current new position.
		ptr = current_pos->first_edge;
		// If the stack has the same number of vertexes as the graph, then exit.
		if (stack->size == limit) {
			exit_flag = TRUE;
		}
	}
	// Print and free stack.
	print_stack(stack, FALSE);
	free_stack(stack);
}

// Helper function for BFS part 2.
void explore_bfs(Graph* graph, Stack *stack, Edge *ptr) {
	while (ptr != NULL) {
		// Iterate through pointers and check edge hasn't been 'seen'. If they haven't been added,
		// add to stack.
		if (check_stack(stack, ptr) == TRUE) {
			stack_add_end(stack, ptr->v, ptr->weight, FALSE, graph->vertices[ptr->v]->label);
			ptr = ptr->next_edge;
		}
		else {
			ptr = ptr->next_edge;
		}
	}
}

// PART 3
void detailed_path(Graph* graph, int source_id, int destination_id) {
	// Create a new stack to keep track of the path
	int exit_flag = FALSE;
	Stack *stack = new_stack();
	Stack *path = new_stack();
	Vertex *current_pos = graph->vertices[source_id];
	Edge *ptr = current_pos->first_edge;
	// Add the source_id to stack
	stack_add_end(stack, ptr->u, FALSE, TRUE, graph->vertices[ptr->u]->label);
	// Call the recursive depth-first search funciton. Initiate the one_path variable as true,
	// returning as soon as the destination is reached.
	// (printing and freeing is done in-function)
	explore_dfs_all(graph, stack, path, current_pos, destination_id, TRUE, &exit_flag);
	// Free stack.
	print_stack(path, TRUE);
	free_stack(path);
	free_stack(stack);
	return;
}

// Helper function for part 3 and 4. Variables one_path, path and exit_flag are for part 3 use.
void explore_dfs_all(Graph *graph, Stack *stack, Stack *path, Vertex *current_pos, int destination_id, 
						int one_path, int *exit_flag) {
	Edge *ptr = current_pos->first_edge;
	if (*exit_flag == TRUE) {
		return;
	}
	// Add the current position to the stack
	// If we've reached the final destination
	if (ptr->u == destination_id) {
		// print the path
		if (one_path == TRUE) {
			// For part 3, print stack. Stop recursive function calls.
			Node *nodeptr = stack->head;
			while (nodeptr != NULL) {
				stack_add_end(path, nodeptr->id, nodeptr->distance, TRUE, nodeptr->label);
				nodeptr = nodeptr->next;
			}
			*exit_flag = TRUE;
			return;
		}
		else {
			// For part 4, just output the line
			print_oneline(stack, NO_DIST);
		}	
	}
	else {
		// if not at the final destination,
		// call the function to all valid vertexes
		while (ptr != NULL) {
			// check stack if vertex is valid
			if (check_stack(stack, ptr) == TRUE){
				// Add vertex to stack
				stack_add_end(stack, ptr->v, ptr->weight, TRUE, graph->vertices[ptr->v]->label);
				explore_dfs_all(graph, stack, path, graph->vertices[ptr->v], destination_id, 
					one_path, exit_flag);
			}
			ptr = ptr->next_edge;
		}
	}
	// Pop the last vertex from the stack
	stack_remove_end(stack);

}

// PART 4
void all_paths(Graph* graph, int source_id, int destination_id) {
	// Create a new stack to contain the current path
	int exit_flag = FALSE;	
	Stack *path = new_stack();
	Stack *none = new_stack();
	Vertex *current_pos = graph->vertices[source_id];
	Edge *ptr = current_pos->first_edge;
	// Add the source_id vertex to the stack.
	stack_add_end(path, ptr->u, FALSE, TRUE, graph->vertices[ptr->u]->label);
	// Call the recursive depth-first search funciton. Initiate one_path as false.
	explore_dfs_all(graph, path, none, current_pos, destination_id, FALSE, &exit_flag);
	// free the stack.
	free_stack(path);
}


// Prints the path as one line with distance.
void print_oneline(Stack *stack, int distance) {
	// Start at the head of the stack and iterate until null, printing each vertex with a comma
	Node *nodeptr = stack->head;
	while (nodeptr->next != NULL) {
		printf("%s, ", nodeptr->label);
		nodeptr = nodeptr->next;
	}
	// If the distance option is true, then print the distance (to be used in part 5)
	if (distance != NO_DIST) {
		printf("%s (%dkm)\n", nodeptr->label, distance);
	}
	else {
		printf("%s\n", nodeptr->label);
	}
}

// PART 5
void shortest_path(Graph* graph, int source_id, int destination_id) {
	// Initialise the largest distance as INT_MAX
	int shortest = INT_MAX;

	// Create two stacks, one to keep the current path and one to keep the shortest path
	Stack *path = new_stack();
	Stack *shortest_path = new_stack();
	Vertex *current_pos = graph->vertices[source_id];

	// Call the first instance of the recursive DFS algorithm
	explore_dfs_shortest(graph, path, shortest_path, current_pos, destination_id, &shortest, FALSE);
	// Print the shortest path and then free the temp path and the shortest path
	if (source_id == destination_id) {
		print_oneline(shortest_path, FALSE);
	}
	else {
		print_oneline(shortest_path, shortest);
	}	
	free_stack(path);
	free_stack(shortest_path);
}

// Helper function for part 5
void explore_dfs_shortest(Graph *graph, Stack *stack, Stack *shortest_path, Vertex *current_pos, 
							int destination_id, int *shortest, int prev_weight) {
	Edge *ptr = current_pos->first_edge;
	// Add the current position to the stack
	stack_add_end(stack, ptr->u, prev_weight, TRUE, graph->vertices[ptr->u]->label);
	Node *nodeptr = stack->head;
	
	// If we've reached the final destination
	if (current_pos->first_edge->u == destination_id) {
		// Calculate the distance of the current path
		int current_dist = calculate_dist(stack);

		// If the current path is shorter than the shortest recorded path, 
		// then reset the shortest path and copy the current path into the shortest path
		if (*shortest > current_dist) {
			free_stack(shortest_path);
			Stack *shortest_path = new_stack();
			*shortest = current_dist;
			while (nodeptr != NULL) {
				stack_add_end(shortest_path, nodeptr->id, nodeptr->distance, TRUE, nodeptr->label);
				nodeptr = nodeptr->next;
			}
		}
	}
	else {
		// if not at the final destination,
		// call the function to all valid vertexes
		while (ptr != NULL) {
			if (check_stack(stack, ptr) == TRUE){
				explore_dfs_shortest(graph, stack, shortest_path, graph->vertices[ptr->v], 
										destination_id, shortest, ptr->weight);
			}
			ptr = ptr->next_edge;
		}
	}
	// Pop the last vertex from the stack
	stack_remove_end(stack);
}

// Calculates distance of a path
int calculate_dist(Stack *stack) {
	// Iterate through stack and calculate total distance
	int dist_sum = 0;
	Node *nodeptr = stack->head;
	while (nodeptr != NULL) {
		dist_sum += nodeptr->distance;
		nodeptr = nodeptr->next;
	}
	return dist_sum;
}

// Prints stack with newlines
void print_stack(Stack *stack, int detail){
	// Iterates through stack and prints each vertex.
	// If detail is TRUE, then calculate the cumulative distance 
	// and print as each vertex is reached.
	int dist_sum = 0;
	if (detail == TRUE) {
		Node *nodeptr = stack->head;
		while(nodeptr != NULL) {
			dist_sum += nodeptr->distance;
			printf("%s (%dkm)", nodeptr->label, dist_sum);
			printf("\n");
			nodeptr = nodeptr->next;
		}
	}
	else {
		Node *nodeptr = stack->head;
		while(nodeptr != NULL) {
			printf("%s\n", nodeptr->label);
			nodeptr = nodeptr->next;
		}
	}
}

// Function checks the stack if the pointer has already been added and returns TRUE accordingly
int check_stack(Stack *stack, Edge *ptr) {
	// Iterates through stack and checks if the specified vertex is valid.
	Node *nodeptr = stack->head;
	while (nodeptr != NULL) {
		if (nodeptr->id == ptr->v) {
			return FALSE;
		}
		nodeptr = nodeptr->next;
	}
	return TRUE;
}

/* Algorithms are Fun! */