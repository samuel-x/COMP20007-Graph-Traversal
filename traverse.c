/* ... */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "traverse.h"
#include "list.h"
//typedef struct node Node;

//typedef struct list List;

int check_list(List *list, Edge *ptr);
int travel_bfs(Graph* graph, int current_id, int* seen, int size);
int all_path_traverse(Graph* graph, int source_id, int destination_id, List* list);

void print_dfs(Graph* graph, int source_id) {
	int limit = graph->n;
	List *list = new_list();
	Vertex *current_pos = graph->vertices[source_id];
	Edge *ptr = current_pos->first_edge;
	int exit_flag = 0;
	int added_id = -1;
	Node *nodeptr;


	while (exit_flag != 1) {
		while (ptr != NULL) {
			if (list_is_empty(list) == 1) {
				list_add_end(list, ptr->u, 0, 1, graph->vertices[ptr->u]->label);
				nodeptr = list->head;
			}
			else {
				if (check_list(list, ptr) == 1) {
					list_add_end(list, ptr->v, ptr->weight, 1, graph->vertices[ptr->v]->label);
					added_id = ptr->v;
					break;
				}
				else {
					ptr = ptr->next_edge;
				}
			}
		}
		if (added_id == -1) {
			list_add_end(list, ptr->v, ptr->weight, 1, graph->vertices[ptr->v]->label);
			added_id = ptr->v;
		}
		current_pos = graph->vertices[added_id];
		ptr = current_pos->first_edge;
		
		nodeptr = nodeptr->next;
		added_id = -1;

		if (list->size == limit) {
			exit_flag = 1;
		}
	}
	nodeptr = list->head;
	while(nodeptr != NULL) {
		printf("%s\n", nodeptr->label);
		nodeptr = nodeptr->next;
	}
	free_list(list);
}
	// Iterate until all all vertexes have been seen
	/*
	for (i=0; i<limit; i++) {
		while(ptr != NULL && save == -1){
			flag = 0;
			for (j=0; j<size; j++) {
			// iterate through villages visited and check if they've been visited
				if (ptr->v == seen[j]) {
					flag = 1;
					continue;
				}
			}
			if (flag == 1) {
				ptr = ptr->next_edge;
			}
			else {
				save = ptr->v;
				break;
			}
		}
		if (flag == 1) {
			save = ptr->v;
		}
		// Add visited village to list of villages visited and print village name
		printf("%s\n", current_pos->label);
		seen[size] = current_pos->first_edge->u;
		size++;
		//go to the next vertex
		current_pos = graph->vertices[save];
		//reset the pointer to the first edge
		ptr = current_pos->first_edge;
		save = -1;
		//printf("You are now in: %s\n", current_pos->label);
	}
}
*/
void print_bfs(Graph* graph, int source_id) {
	//redo with recursion
	int limit = graph->maxn;
	List *list = new_list();
	Vertex *current_pos = graph->vertices[source_id];
	Edge *ptr = current_pos->first_edge;
	int exit_flag = 0;
	//printf("%s\n", current_pos->label);
	//list_add_end(list, ptr->u, 1, graph->vertices[ptr->v]->label);
	//Node *nodeptr = list->head;
	Node *nodeptr;
	while (exit_flag != 1) {
		while (ptr != NULL) {
			if (list_is_empty(list) == 1) {
				list_add_end(list, ptr->u, 0, 1, graph->vertices[ptr->u]->label);
				nodeptr = list->head;
			}
			else {
				if (check_list(list, ptr) == 1) {
					list_add_end(list, ptr->v, ptr->weight, 0, graph->vertices[ptr->v]->label);
					//printf("Added to List!\n");
					//printf("%s\n", graph->vertices[ptr->v]->label);
					ptr = ptr->next_edge;
				}
				else {
					ptr = ptr->next_edge;
				}
			}
		}
		if (nodeptr->visited == 0) {
			current_pos = graph->vertices[nodeptr->id];
			nodeptr->visited = 1;
			nodeptr = nodeptr->next;
		}
		else {
			nodeptr = nodeptr->next;
		}
		ptr = current_pos->first_edge;
		if (list->size == limit) {
			exit_flag = 1;
		}
	}
	nodeptr = list->head;
	while(nodeptr != NULL) {
		printf("%s\n", nodeptr->label);
		nodeptr = nodeptr->next;
	}
	free_list(list);
	//travel_bfs(graph, source_id, seen, size);
}

int check_list(List *list, Edge *ptr) {
	Node *nodeptr = list->head;
	while (nodeptr != NULL) {
		//printf("Checking %d vs %d\n", nodeptr->id, ptr->v);
		if (nodeptr->id == ptr->v) {
			//printf("Match found in list!\n");
			return 0;
		}
		nodeptr = nodeptr->next;
	}
	return 1;
}


void detailed_path(Graph* graph, int source_id, int destination_id) {
	int limit = graph->n;
	List *list = new_list();
	int size = 0;
	Vertex *current_pos = graph->vertices[source_id];
	Edge *ptr = current_pos->first_edge;
	int exit_flag = 0;
	int added_id = -1;
	int dist_sum = 0;
	Node *nodeptr;


	while (exit_flag != 1) {
		while (ptr != NULL) {
			if (list_is_empty(list) == 1) {
				list_add_end(list, ptr->u, 0, 1, graph->vertices[ptr->u]->label);
				nodeptr = list->head;
			}
			else {
				if (check_list(list, ptr) == 1) {
					list_add_end(list, ptr->v, ptr->weight, 1, graph->vertices[ptr->v]->label);
					added_id = ptr->v;
					break;
				}
				else {
					ptr = ptr->next_edge;
				}
			}
		}
		if (added_id == -1) {
			list_add_end(list, ptr->v, ptr->weight, 1, graph->vertices[ptr->v]->label);
			added_id = ptr->v;
		}
		if (list->size == limit || added_id == destination_id) {
			exit_flag = 1;
		}
		current_pos = graph->vertices[added_id];
		ptr = current_pos->first_edge;
		
		nodeptr = nodeptr->next;

		added_id = -1;
	}
	nodeptr = list->head;
	while(nodeptr != NULL) {
		dist_sum += nodeptr->distance;
		printf("%s (%dkm)\n", nodeptr->label, dist_sum);
		nodeptr = nodeptr->next;
	}
	free_list(list);
}
/*
	int *seen;
    limit = graph->n;
	int size = 0;
	int cumulative = 0;
	int i, j;
	seen = malloc(limit*sizeof(int));
	memset(seen, 0, sizeof(int));
    current_pos = graph->vertices[source_id];
	ptr = current_pos->first_edge;
	int save = -1;
	int save_w = 0;
	exit_flag = 0;
	int flag;
	// Iterate until all all vertexes have been seen
	for (i=0; (i<limit) && (exit_flag != 1); i++) {
		// check if arrived in destination. If so, continue final loop to calculate the total distance.
		if (current_pos->first_edge->u == graph->vertices[destination_id]->first_edge->u) {
			exit_flag = 1;
		}
		// Iterate through edges and check if they have been visited.
		while(ptr->next_edge != NULL && save == -1){
			flag = 0;
			for (j=0; j<size; j++) {
			// iterate through villages visited and check if they've been visited
				if (ptr->v == seen[j]) {
					flag = 1;
					continue;
				}
			}
			if (flag == 1) {
				ptr = ptr->next_edge;
			}
			else {
				save = ptr->v;
				save_w = ptr->weight;
				break;
			}
		}
		// If all of the edges have already been travelled to, then return to the first village.
		if (flag == 1) {
			save = ptr->v;
			save_w = ptr->weight;
		}
		// print to stdout
		printf("%s (%dkm)\n", current_pos->label, cumulative);
		// Add up distance travelled
		cumulative += save_w;
		// Add visited village to list of villages visited.
		seen[size] = current_pos->first_edge->u;
		size++;
		//go to the next vertex
		current_pos = graph->vertices[save];
		//reset the pointer to the first edge
		ptr = current_pos->first_edge;
		// reset the condition
		save = -1;
	}
}
*/
void all_paths(Graph* graph, int source_id, int destination_id) {
	int limit = graph->n;
	List *list = new_list();
	Vertex *current_pos = graph->vertices[source_id];
	Edge *ptr = current_pos->first_edge;
	int exit_flag = 0;
	int added_id = -1;
	Node *nodeptr;


	while (exit_flag != 1) {
		while (ptr != NULL) {
			if (list_is_empty(list) == 1) {
				list_add_end(list, ptr->u, 0, 1, graph->vertices[ptr->u]->label);
				nodeptr = list->head;
			}
			else {
				if (check_list(list, ptr) == 1) {
					list_add_end(list, ptr->v, ptr->weight, 1, graph->vertices[ptr->v]->label);
					added_id = ptr->v;
					break;
				}
				else {
					ptr = ptr->next_edge;
				}
			}
		}
		if (added_id == -1) {
			list_add_end(list, ptr->v, ptr->weight, 1, graph->vertices[ptr->v]->label);
			added_id = ptr->v;
		}
		current_pos = graph->vertices[added_id];
		ptr = current_pos->first_edge;
		
		nodeptr = nodeptr->next;
		added_id = -1;

		if (list->size == limit) {
			exit_flag = 1;
		}
	}
	nodeptr = list->head;
	while(nodeptr != NULL) {
		printf("%s\n", nodeptr->label);
		nodeptr = nodeptr->next;
	}
	free_list(list);
}

void shortest_path(Graph* graph, int source_id, int destination_id) {
	printf("not yet implemented: put code for part 5 here\n");
}
/*
// helper function for part 2
int travel_bfs(Graph* graph, int current_id, int* seen, int size) {
	int i, flag = 0; //edges=0;
	Edge* ptr = graph->vertices[current_id]->first_edge;
	while (ptr->next_edge != NULL) {
		for (i=0; i<size; i++) {
			if (ptr->v == seen[i]) {
				flag = 1;
			}
		}
		if (flag == 1){
			ptr = ptr->next_edge;
		}
		else {
			//edges++;
			seen[size] = ptr->v;
			size++;
			ptr->next_edge;
			printf("%s\n", graph->vertices[ptr->v]->label);
			printf("%d\n", size);
		}	
		flag = 0;
	}
	ptr = graph->vertices[ptr->u]->first_edge;
	if (ptr->next_edge = NULL) {
		return 0;
	}
	else {
		travel_bfs(graph, graph->vertices[ptr->v], seen, size);
	}
}

*/
/*
int all_path_traverse(Graph* graph, int source_id, int destination_id, List* list){
	int limit = graph->n;
	int cumulative = 0;
	int i, j;
	Vertex *current_pos = graph->vertices[source_id];
	Edge *ptr = current_pos->first_edge;
	int save = -1;
	int save_w = 0;
	int exit_flag = 0;
	int flag;
	// Iterate until all all vertexes have been seen
	for (i=0; (i<limit) && (exit_flag != 1); i++) {
		// check if arrived in destination. If so, continue final loop to calculate the total distance.
		if (current_pos->first_edge->u == graph->vertices[destination_id]->first_edge->u) {
			exit_flag = 1;
		}
		// Iterate through edges and check if they have been visited.
		while(ptr->next_edge != NULL && save == -1){
			flag = 0;
			if (check(ptr->next_edge, list, i) == 1) {
				flag = 1;
				continue;
			}
			if (flag == 1) {
				ptr = ptr->next_edge;
			}
			else {
				save = ptr->v;
				break;
			}
		}
		// If all of the edges have already been travelled to, then just go to the last village.
		if (flag == 1) {
			save = ptr->v;
		}
		// print to stdout
		printf("%s ", current_pos->label);
		// Add up distance travelled
		// Add visited village to list of villages visited.
		list_add_end(list, current_pos->first_edge->u, 1, current_pos->label);
		//go to the next vertex
		current_pos = graph->vertices[save];
		//reset the pointer to the first edge
		ptr = current_pos->first_edge;
		// reset the condition
		save = -1;
	}
	printf("\n");
	return 0;
}

int check(int pos_id, List* list, int position) {
	Node *nodeptr = list->head;
	int i = 0;
	while(nodeptr->next != NULL && i < position) {
		if (pos_id == nodeptr->id) {
			return 1;
		}
	}
	return 0;
}
*/