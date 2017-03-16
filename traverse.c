/* ... */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "traverse.h"

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
	int *seen;
	int limit = graph->n;
	int size = 0;
	int edges = 0;
	int i, j, save, flag = 0;
	seen = malloc(limit*sizeof(int));
	memset(seen, 0, sizeof(int));
	Vertex *current_pos = graph->vertices[source_id];
	Edge *ptr = current_pos->first_edge;
	Vertex *prev_pos = graph->vertices[source_id];
	Edge *prev_ptr = current_pos->first_edge;
	// Iterate until all all vertexes have been seen
	seen[size] = source_id;
	size++;

	for (i=0; i<limit; i++) {
		// iterate through all edges for current point
		while(ptr->next_edge != NULL) {
			// count edges
			edges++;
			// check if edge has already been to
			
		}
	}


	for (i=0; i<size; i++) {
		printf("%s\n", graph->vertices[seen[i]]->label);
	}
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
