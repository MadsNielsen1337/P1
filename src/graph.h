#ifndef P1_GRAPH_H
#define P1_GRAPH_H

#include "routes.h"
#include "trains.h"

// Define the maximum number of vertices in the graph. Should use list_length from routes.h after a station list is built!!
#define N 6

// Data structure to store adjacency list nodes of the graph
struct Node
{
    int dest, weight;
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
};

// Function to create an adjacency list from specified edges
struct Graph* createGraph(struct Edge edges[], int n);

// Function to print adjacency list representation of a graph
void printGraph(struct Graph* graph);

// Function that constructs the edges for the graph
struct Edge* build_edges(station_list_node* list, route* r, int route_count, train* t);


#endif //P1_GRAPH_H
