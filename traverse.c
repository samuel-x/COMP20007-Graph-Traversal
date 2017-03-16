/* ... */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "traverse.h"

void travel_bfs(Graph* graph, int current_id, int* seen, int size);

void print_dfs(Graph* graph, int source_id) {
	int *seen;
	int limit = graph->n;
	int size = 0;
	int i, j;
	seen = malloc(limit*sizeof(int));
	memset(seen, 0, sizeof(int));
	Vertex *current_pos = graph->vertices[source_id];
	Edge *ptr = current_pos->first_edge;
	int save = -1;
	int flag;
	// Iterate until all all vertexes have been seen
	for (i=0; i<limit; i++) {
		while(ptr->next_edge != NULL && save == -1){
			//printf("You are in: %d:%s\t\t\tchecking %d:%s.\t\tDistance: %d\n", ptr->u, current_pos->label, ptr->v, graph->vertices[ptr->v]->label, ptr->weight);
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

void print_bfs(Graph* graph, int source_id) {
	//redo with recursion
	int *seen;
	int limit = graph->maxn;
	int flag = 0;
	int size = 0;
	seen = malloc(limit*sizeof(int));
	travel_bfs(graph, source_id, seen, size);
}


void detailed_path(Graph* graph, int source_id, int destination_id) {
	int *seen;
	int limit = graph->n;
	int size = 0;
	int cumulative = 0;
	int i, j;
	seen = malloc(limit*sizeof(int));
	memset(seen, 0, sizeof(int));
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

void all_paths(Graph* graph, int source_id, int destination_id) {
	printf("not yet implemented: put code for part 4 here\n");
}

void shortest_path(Graph* graph, int source_id, int destination_id) {
	printf("not yet implemented: put code for part 5 here\n");
}

// helper function for part 2
void travel_bfs(Graph* graph, int current_id, int* seen, int size) {
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
		return;
	}
	else {
		travel_bfs(graph, graph->vertices[ptr->v], seen, size);
	}
}