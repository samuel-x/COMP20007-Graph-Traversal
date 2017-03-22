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
int all_path_traverse(Graph* graph, int source_id, int destination_id, List* list);
void explore_dfs(Graph* graph, List *list, Edge *ptr, int *added_id, int *branch);
void explore_bfs(Graph* graph, List *list, Edge *ptr);
void print_list(List *list, int detail);

//PART 1
void print_dfs(Graph* graph, int source_id) {
	int limit = graph->n;
	List *list = new_list();
	Vertex *current_pos = graph->vertices[source_id];
	Edge *ptr = current_pos->first_edge;
	int exit_flag = 0;
	int added_id = NO_ID;
	int branch = 0;
	int i;
	Node *nodeptr;

	list_add_end(list, ptr->u, FALSE, TRUE, graph->vertices[ptr->u]->label);
	nodeptr = list->head;
	while (exit_flag != TRUE) {
		explore_dfs(graph, list, ptr, &added_id, &branch);
		if (added_id == NO_ID) {
			nodeptr = list->head;
			for (i = 0; i<(list->size - branch)-1; i++) {
				nodeptr = nodeptr->next;
			}
			added_id = nodeptr->id;
			branch = 0;
			//list_add_end(list, ptr->v, ptr->weight, FALSE, graph->vertices[ptr->v]->label);
			//added_id = ptr->v;
		}	
		//printf("currently at: %s\n", current_pos->label);
		current_pos = graph->vertices[added_id];
		ptr = current_pos->first_edge;
		
		nodeptr = nodeptr->next;
		added_id = NO_ID;
		branch++;
		//printf("list->size: %d vs limit: %d, %d\n", list->size, limit, size);
		if (list->size >= limit || branch >= limit) {
			exit_flag = TRUE;
		}
	}
	//printf("successfully traversed\n");
	print_list(list, FALSE);
}

void explore_dfs(Graph* graph, List *list, Edge *ptr, int *added_id, int *branch) {
	while (ptr != NULL) {
		if (check_list(list, ptr) == TRUE) {
			if (*added_id != NO_ID) {
				*branch = 0;
				break;
			}
			//printf("checking: %s\n", graph->vertices[ptr->v]->label);
			list_add_end(list, ptr->v, ptr->weight, FALSE, graph->vertices[ptr->v]->label);
			*added_id = ptr->v;
		}
		else {
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


void all_paths(Graph* graph, int source_id, int destination_id) {
	//int limit = graph->n;
	//List *list = new_list(limit);
	//all_path_traverse(graph, source_id, destination_id, list);
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
			printf("%s", nodeptr->label);
			printf(" (%ikm)", dist_sum);
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