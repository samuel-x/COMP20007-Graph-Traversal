/* ... */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "traverse.h"
#include "list.h"
//typedef struct node Node;
#define FALSE 0
#define TRUE 1
#define NO_ID -1
//typedef struct list List;

int check_list(List *list, Edge *ptr);
void explore_dfs(Graph* graph, List *list, Edge *ptr, int *added_id, int *branch);
void explore_bfs(Graph* graph, List *list, Edge *ptr);
void explore_dfs_all(Graph* graph, List *list_x, List *list_y, List *path, Edge *ptr, int *added_id, int *branch);
void print_list(List *list, int detail);
int check_path(List *list_x, List *list_y, List *path, Edge *ptr);

//PART 1
void print_dfs(Graph* graph, int source_id) {
	int limit = graph->n;
	// Create a new list to store towns travelled to
	List *list = new_list();
	Vertex *current_pos = graph->vertices[source_id];
	Edge *ptr = current_pos->first_edge;
	int exit_flag = 0;
	int added_id = NO_ID;
	int branch = 0;
	int i;
	Node *nodeptr;

	// Add the first village to list (source location)
	list_add_end(list, ptr->u, FALSE, TRUE, graph->vertices[ptr->u]->label);
	nodeptr = list->head;

	// While graph has not fully been traversed, explore until it has
	while (exit_flag != TRUE) {
		// Explore function which checks which path to follow. If no path is found (as in all edges have been travelled to,
		// make added_id NO_ID)
		explore_dfs(graph, list, ptr, &added_id, &branch);
		// If no path was found, then backtrack until new path is found.
		if (added_id == NO_ID) {
			nodeptr = list->head;
			for (i = 0; i<(list->size - branch)-1; i++) {
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
		
		// If the same amount of villages as in the graph have been travelled to, then exit successfully.
		if (list->size >= limit || branch >= limit) {
			exit_flag = TRUE;
		}
	}
	// print the full list (with no distance measures)
	print_list(list, FALSE);
}

void explore_dfs(Graph* graph, List *list, Edge *ptr, int *added_id, int *branch) {
	// check all edges
	while (ptr != NULL) {
		// if the edge is in the list,
		if (check_list(list, ptr) == TRUE) {
			// check if there is currently already a vertex found, if so, then reset the backtrack counter
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
	List *list = new_list();
	Vertex *current_pos = graph->vertices[source_id];
	Edge *ptr = current_pos->first_edge;
	int exit_flag = 0;
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
	List *list = new_list();
	int size = 0;
	Vertex *current_pos = graph->vertices[source_id];
	Edge *ptr = current_pos->first_edge;
	int exit_flag = 0;
	int added_id = NO_ID;
	int branch = 0, i = 0;
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
	int limit = graph->n;
	List *list_x = new_list();
	List *list_y = new_list();
	List *path = new_list();
	int size = 0;
	Vertex *current_pos = graph->vertices[source_id];
	Edge *ptr = current_pos->first_edge;
	int exit_flag = 0;
	int added_id = NO_ID;
	int branch = 0, i = 0;
	Node *nodeptr;
	Node *pathptr;


	list_add_end(path, ptr->u, FALSE, TRUE, graph->vertices[ptr->u]->label);
	list_x->head = path->head;
	list_y->head = path->head;

	pathptr = path->head;
	nodeptr = list_x->head;

	while (exit_flag != TRUE) {
		explore_dfs_all(graph, list_x, list_y, path, ptr, &added_id, &branch);
		if (added_id == NO_ID) {
			pathptr = path->head;
			for (i = 0; i<(path->size - branch)-1; i++) {
				pathptr = pathptr->next;
			}
			added_id = pathptr->id;
			branch = 0;
		}
		if (added_id == destination_id) {
			print_list(path, FALSE);
			printf("\n");
			current_pos = graph->vertices[source_id];
			ptr = current_pos->first_edge;
			List *path = new_list();
			list_add_end(path, ptr->u, FALSE, TRUE, graph->vertices[ptr->u]->label);
			list_add_end(list_y, ptr->u, FALSE, TRUE, graph->vertices[ptr->u]->label);
			list_y->tail = path->head;
			list_x->next_list = list_y->tail;
			pathptr = path->head;
			//printf("%s", pathptr->label);
			branch = 0;
			added_id = NO_ID;
			continue;
		}
		if (pathptr == NULL) {
			exit_flag = TRUE;
			break;
		}
		//printf("%s", current_pos->label);
		current_pos = graph->vertices[added_id];
		ptr = current_pos->first_edge;
		branch++;
		pathptr = pathptr->next;
		added_id = NO_ID;
	}
	//print_list(list, TRUE);
	//free_list(list);
}

void explore_dfs_all(Graph* graph, List *list_x, List *list_y, List *path, Edge *ptr, int *added_id, int *branch) {
	// check all edges
	while (ptr != NULL) {
		// if the edge is in the list,
		if (check_path(list_x, list_y, path, ptr) == TRUE && check_list(path, ptr) == TRUE) {
			// check if there is currently already a vertex found, if so, then reset the backtrack counter
			// (meaning you can revert to this vertex if you end up at a dead end)
			if (*added_id != NO_ID) {
				*branch = 0;
				break;
			}
			printf("Adding %s\n", graph->vertices[ptr->v]->label);
			// Add vertex to list if not currently in list
			list_add_end(path, ptr->v, ptr->weight, FALSE, graph->vertices[ptr->v]->label);
			*added_id = ptr->v;
		}
		else {
			// If edge is in the list, then just go to the next edge.
			ptr = ptr->next_edge;
		}
	}
}

void shortest_path(Graph* graph, int source_id, int destination_id) {
	printf("not yet implemented: put code for part 5 here\n");
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

int check_path(List *list_x, List *list_y, List *path, Edge *ptr) {
	Node *nodeptr = list_x->head;
	Node *pathptr = path->head;
	int valid = TRUE;
	int check = FALSE;
	int i = 0;
	while (nodeptr != NULL && pathptr != NULL && i<list_y->size) {
		printf("Checking %s vs %s\n", nodeptr->label, pathptr->label);
		if (nodeptr->id == pathptr->id) {
			check = TRUE;
		}
		else {
			check = FALSE;
		}
		if (nodeptr->next == NULL) {
			printf("CHECK: %d\n", check);
			if (check == TRUE) {
				valid = FALSE;
				break;
			}	
			else {
				printf("%d ", i);
				i++;
				nodeptr = list_x->next_list;
				check = FALSE;
				continue;
			}
		}
		nodeptr = nodeptr->next;
		pathptr = pathptr->next;
	}
	printf("VALID: %d\n", valid);
	return valid;
}