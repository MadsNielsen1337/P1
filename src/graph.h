#ifndef P1_GRAPH_H
#define P1_GRAPH_H

#include "routes.h"
#include "trains.h"

// Define the maximum number of vertices in the graph. Should use list_length from routes.h after a station list is built!!
#define N 100

// Data structure to store adjacency list nodes of the graph
struct Node
{
    int dest, weight;
    char* allowed_trains;
    struct Node* next;
};

// Data structure to store graph
struct Graph
{
    // An array of pointers to `Node` for representing adjacency list
    struct Node* head[N];
};

// Data structure to store graph edges
struct Edge {
    int src, dest, weight;
    char trains[XL_DATA_SIZE];
};

// Function to create an adjacency list from specified edges
struct Graph* createGraph(struct Edge edges[], int route_count);

// Function to print adjacency list representation of a graph
void printGraph(struct Graph* graph);

// Function that constructs the edges for the graph
struct Edge* build_edges(station_list_node* list, route* r, int route_count, train* t, int train_count);

//int dijkstra(struct Graph* graph, int start_node, int end_node);

//int train_match(struct Graph* graph, int search_pos);

void dijkstra(struct Graph* graph, float* distance, int* previous_node, int start_node, int node_count);

#endif //P1_GRAPH_H
