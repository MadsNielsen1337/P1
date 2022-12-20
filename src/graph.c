#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "graph.h"
#include "trains.h"
#include "time_calc.h"

// Function to create an adjacency list from specified edges
struct Graph* createGraph(struct Edge edges[], int route_count)
{
    unsigned i;
    // allocate memory for the graph data structure
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));

    // initialize head pointer for all vertices
    for (i = 0; i < N; i++) {
        graph->head[i] = NULL;
    }

    // add edges to the directed graph one by one
    for (i = 0; i < route_count; i++)
    {
        // get the source and destination vertex
        int src = edges[i].src;
        int dest = edges[i].dest;
        int weight = edges[i].weight;
        char* allowed_trains;
        allowed_trains = edges[i].trains;

        // allocate new node of adjacency list from `src` to `dest`
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->dest = dest;
        newNode->weight = weight;
        newNode->allowed_trains = allowed_trains;

        // point new node to current head
        newNode->next = graph->head[src];

        // point head pointer to a new node
        graph->head[src] = newNode;

        // allocate new node of adjacency list from `dest` to `src`
        newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->dest = src;
        newNode->weight = weight;
        newNode->allowed_trains = allowed_trains;

        // point new node to current head
        newNode->next = graph->head[dest];

        // change head pointer to point to the new node
        graph->head[dest] = newNode;
    }

    return graph;
}
// Function to print adjacency list representation of a graph
void printGraph(struct Graph* graph)
{
    int i;
    printf("\n");
    for (i = 0; i < N; i++)
    {
        // print the current vertex and all its neighbors
        struct Node* ptr = graph->head[i];
        while (ptr != NULL)
        {
            printf("%d -> %d (%d)   \t", i, ptr->dest, ptr->weight);
            ptr = ptr->next;
        }
        printf("\n");
    }
}

// Function that constructs the edges for the graph
struct Edge* build_edges(station_list_node* list, route* r, int route_count, train* t, int train_count){
    struct Edge* edges = malloc(sizeof(struct Edge) * route_count);
    char* allowed_trains;

    for (int i = 0; i < route_count; ++i) {
        edges[i].src = search_station_list(list, r[i].station_start);
        edges[i].dest = search_station_list(list, r[i].station_end);
        edges[i].weight = weight_calc(r[i], *t);
        allowed_trains = compatible_trains(t, r[i], train_count);
        strcpy(edges[i].trains , allowed_trains);
        //printf("%s\n Works on src %s & dest %s\n\n", edges[i].trains, index_station_list(list, edges[i].src), index_station_list(list, edges[i].dest));
        free(allowed_trains);
    }
    return edges;
}

/**
 * function that finds the shortest possible path from a given node in the graph to every other reachable position
 * @param graph is the graph struct that holds the graph to be searched
 * @param dist is the array which the function will write the distances into. The index to a given distance corresponds to the node that is reached in that distance. It should be as long as the number of nodes in the graph.
 * @param prev is an array which the function will write the previous node to the node represented by the index value into. Its size should match @param dist
 * @param start_node is the integer corresponding to the node from which to measure distances from.
 * @param node_count is the number of nodes in the graph. This corresponds to the length of the linked station list.
 */
void dijkstra(struct Graph* graph, float* dist, int* prev, int start_node, int node_count) {
    float queue[node_count];
    for (int i = 0; i < node_count; ++i) {
        queue[i] = INFINITY;
    }

    for (int i = 0; i < node_count; ++i) {  //setup initial distances to all vertices i. these are infinite to signal that they have not been visited yet
        dist[i] = INFINITY;
        prev[i] = -1;                       //no paths exist yet, so the previous node to any other is nonexistent, represented by -1
    }
    dist[start_node] = 0;               //we begin at this node, so the distance is 0
    prev[start_node] = -1;              //and there is no previous node to the first one, so we set it to -1

    for (int i = 0; i < node_count; ++i) {
        for (int j = 0; j < node_count; ++j) {
            if(!isnan(queue[j])){
                queue[j] = dist[j];     //copying all entries in the distance array into the queue unless that spot in the queue is NaN
            }
        }
        float temp = INFINITY;
        int search_pos;
        for (int j = 0; j < node_count; ++j) {
            if(queue[j] < temp){
                temp = queue[j];
                search_pos = j;
            }
        }
        queue[search_pos] = NAN;        //set the chosen spot in the queue to NaN, marking it as visited

        struct Node *ptr = graph->head[search_pos];
        while (ptr != NULL) {
            if (dist[search_pos] + (float) ptr->weight < dist[ptr->dest]) {
                dist[ptr->dest] = dist[search_pos] + (float) ptr->weight;
                prev[ptr->dest] = search_pos;
            }
            ptr = ptr->next;
        }
    }
}