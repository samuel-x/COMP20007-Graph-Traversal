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

#include "traverse.h"
#include "list.h"

#define FALSE 0
#define TRUE 1
#define NO_ID -1
#define INT_MAX 99999

int check_list(List *list, Edge *ptr);
void explore_dfs(Graph* graph, List *list, Edge *ptr, int *added_id, int *branch);
void explore_bfs(Graph* graph, List *list, Edge *ptr);
void explore_dfs_all(Graph *graph, List *list, Vertex *current_pos, int destination_id);
void explore_dfs_shortest(Graph *graph, List *list, List *shortest_path, Vertex *current_pos, 
							int destination_id, int *shortest, int prev_weight);
void print_oneline(List *list, int distance);
void print_list(List *list, int detail);
int check_path(List **list, List *path, Edge *ptr, int size);
int calculate_dist(List *list);

//PART 1
void print_dfs(Graph* graph, int source_id) {
	int limit = graph->n;
	int exit_flag = 0;
	int added_id = NO_ID;
	int branch = 0;
	int i;
	// Create a new list to store towns travelled to
	List *list = new_list();
	Vertex *current_pos = graph->vertices[source_id];
	Edge *ptr = current_pos->first_edge;
	Node *nodeptr;

	// Add the first village to list (source location)
	list_add_end(list, ptr->u, FALSE, TRUE, graph->vertices[ptr->u]->label);
	nodeptr = list->head;

	// While graph has not fully been traversed, explore until it has
	while (exit_flag != TRUE) {
		// Explore function which checks which path to follow. If no path is found 
		// (as in all edges have been travelled to, make added_id NO_ID)
		explore_dfs(graph, list, ptr, &added_id, &branch);
		// If no path was found, then backtrack until new path is found.
		if (added_id == NO_ID) {
			nodeptr = list->head;
			for (i = 0; i<((list->size-branch)-1); i++) {
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
		
		// If the same amount of villages as in the graph have been travelled to, 
		// then exit successfully.
		if (list->size>=limit || branch>=limit) {
			exit_flag = TRUE;
		}
	}
	// print the full list (with no distance measures)
	print_list(list, FALSE);
}

// Helper function for DFS (non-recursive).
void explore_dfs(Graph* graph, List *list, Edge *ptr, int *added_id, int *branch) {
	// check all edges
	while (ptr != NULL) {
		// if the edge is in the list,
		if (check_list(list, ptr) == TRUE) {
			// check if there is currently already a vertex found, if so, then reset the 
			// backtrack counter
			// (meaning you can revert to this vertex if you end up at a dead end)
			if (*added_id != NO_ID) {
				*branch = 0;
				break;
			}
			// Add vertex to list if not currently in list
			list_add_end(list, ptr->v, ptr->weight, FALSE, graph->vertices[ptr->v]->label);
			*added_id = ptr->v;
		}
		else {
			// If edge is in the list, then just go to the next edge.
			ptr = ptr->next_edge;
		}
	}
}

// PART 2
void print_bfs(Graph* graph, int source_id) {
	int limit = graph->maxn;
	int exit_flag = 0;
	List *list = new_list();
	Vertex *current_pos = graph->vertices[source_id];
	Edge *ptr = current_pos->first_edge;
	Node *nodeptr;
	list_add_end(list, ptr->u, FALSE, TRUE, graph->vertices[ptr->u]->label);
	nodeptr = list->head;	
	while (exit_flag != TRUE) {
		explore_bfs(graph, list, ptr);
		if (nodeptr->visited == 0) {
			current_pos = graph->vertices[nodeptr->id];
			nodeptr->visited = TRUE;
			nodeptr = nodeptr->next;
		}
		else {
			nodeptr = nodeptr->next;
		}
		ptr = current_pos->first_edge;
		if (list->size == limit) {
			exit_flag = TRUE;
		}
	}
	print_list(list, FALSE);
	free_list(list);
}

void explore_bfs(Graph* graph, List *list, Edge *ptr) {
	while (ptr != NULL) {
		if (check_list(list, ptr) == TRUE) {
			list_add_end(list, ptr->v, ptr->weight, FALSE, graph->vertices[ptr->v]->label);
			ptr = ptr->next_edge;
		}
		else {
			ptr = ptr->next_edge;
		}
	}
}

// PART 3
void detailed_path(Graph* graph, int source_id, int destination_id) {
	int limit = graph->n;
	int size = 0;
	int exit_flag = 0;
	int added_id = NO_ID;
	int branch = 0, i = 0;
	List *list = new_list();
	Vertex *current_pos = graph->vertices[source_id];
	Edge *ptr = current_pos->first_edge;
	Node *nodeptr;


	list_add_end(list, ptr->u, FALSE, TRUE, graph->vertices[ptr->u]->label);
	nodeptr = list->head;
	while (exit_flag != TRUE) {
		explore_dfs(graph, list, ptr, &added_id, &branch);
		if (added_id == NO_ID) {
			nodeptr = list->head;
			for (i = 0; i<(list->size - branch)NO_ID; i++) {
				nodeptr = nodeptr->next;
			}
			added_id = nodeptr->id;
			branch = 0;
		}
		if (list->size == limit || added_id == destination_id) {
			exit_flag = TRUE;
		}
		current_pos = graph->vertices[added_id];
		ptr = current_pos->first_edge;
		branch++;
		nodeptr = nodeptr->next;

		added_id = NO_ID;
	}
	print_list(list, TRUE);
	free_list(list);
}

// PART 4
void all_paths(Graph* graph, int source_id, int destination_id) {
	List *path = new_list();
	// Using a recursive version of DFS.
	Vertex *current_pos = graph->vertices[source_id];
	explore_dfs_all(graph, path, current_pos, destination_id);
}

void explore_dfs_all(Graph *graph, List *list, Vertex *current_pos, int destination_id) {
	Edge *ptr = current_pos->first_edge;
	list_add_end(list, ptr->u, ptr->weight, TRUE, graph->vertices[ptr->u]->label);
	//printf("%s\n", current_pos->label);
	//If we've reached the FINAL DESTINATION
	if (current_pos->first_edge->u == destination_id) {
		//printf("printing...\n");
		print_oneline(list, FALSE);	
	}
	else {
		// if not at the FINAL DESTINATION,
		// Go down all the connected Edges
		while (ptr != NULL) {
			if (check_list(list, ptr) == TRUE){
				explore_dfs_all(graph, list, graph->vertices[ptr->v], destination_id);
			}
			ptr = ptr->next_edge;
		}
	}
	list_remove_end(list);
}

void print_oneline(List *list, int distance) {
	Node *nodeptr = list->head;
	while (nodeptr->next != NULL) {
		printf("%s, ", nodeptr->label);
		nodeptr = nodeptr->next;
	}
	if (distance > 0) {
		printf("%s (%dkm)\n", nodeptr->label, distance);
	}
	else {
		printf("%s\n", nodeptr->label);
	}
}

// PART 5
void shortest_path(Graph* graph, int source_id, int destination_id) {
	int shortest = INT_MAX;
	List *path = new_list();
	List *shortest_path = new_list();
	// Using a recursive version of DFS.
	Vertex *current_pos = graph->vertices[source_id];
	explore_dfs_shortest(graph, path, shortest_path, current_pos, destination_id, &shortest, FALSE);
	print_oneline(shortest_path, shortest);	
}

void explore_dfs_shortest(Graph *graph, List *list, List *shortest_path, Vertex *current_pos, 
							int destination_id, int *shortest, int prev_weight) {
	Edge *ptr = current_pos->first_edge;
	list_add_end(list, ptr->u, prev_weight, TRUE, graph->vertices[ptr->u]->label);
	Node *nodeptr = list->head;
	//printf("%s\n", current_pos->label);
	//If we've reached the FINAL DESTINATION
	int current_dist = calculate_dist(list);
	if (current_pos->first_edge->u == destination_id) {
		//printf("printing...\n");
		if (*shortest > current_dist) {
			free(shortest_path);
			List *shortest_path = new_list();
			*shortest = current_dist;
			//printf("Distance: %d\n", *shortest);
			while (nodeptr != NULL) {
				list_add_end(shortest_path, nodeptr->id, nodeptr->distance, TRUE, nodeptr->label);
				nodeptr = nodeptr->next;
			}
		}
	}
	else {
		// if not at the FINAL DESTINATION,
		// Go down all the connected Edges
		while (ptr != NULL) {
			if (check_list(list, ptr) == TRUE){
				explore_dfs_shortest(graph, list, shortest_path, graph->vertices[ptr->v], 
										destination_id, shortest, ptr->weight);
			}
			ptr = ptr->next_edge;
		}
	}
	list_remove_end(list);
}

int calculate_dist(List *list) {
	int dist_sum = 0;
	Node *nodeptr = list->head;
	while (nodeptr != NULL) {
		dist_sum += nodeptr->distance;
		nodeptr = nodeptr->next;
	}
	return dist_sum;
}

void print_list(List *list, int detail){
	int dist_sum = 0;
	if (detail == TRUE) {
		Node *nodeptr = list->head;
		while(nodeptr != NULL) {
			dist_sum += nodeptr->distance;
			printf("%s (%dkm)", nodeptr->label, dist_sum);
			printf("\n");
			nodeptr = nodeptr->next;
		}
	}
	else {
		Node *nodeptr = list->head;
		while(nodeptr != NULL) {
			printf("%s\n", nodeptr->label);
			nodeptr = nodeptr->next;
		}
	}
}

int check_list(List *list, Edge *ptr) {
	Node *nodeptr = list->head;
	while (nodeptr != NULL) {
		//printf("Checking %d vs %d\n", nodeptr->id, ptr->v);
		if (nodeptr->id == ptr->v) {
			//printf("Match found in list!\n");
			return FALSE;
		}
		nodeptr = nodeptr->next;
	}
	return TRUE;
}

/* Algorithms are Fun! */