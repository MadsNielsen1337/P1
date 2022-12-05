#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "time_calc.h"


// Function to create an adjacency list from specified edges
struct Graph* createGraph(struct Edge edges[], int n)
{
    unsigned i;

    // allocate memory for the graph data structure
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));

    // initialize head pointer for all vertices
    for (i = 0; i < N; i++) {
        graph->head[i] = NULL;
    }

    // add edges to the directed graph one by one
    for (i = 0; i < n; i++)
    {
        // get the source and destination vertex
        int src = edges[i].src;
        int dest = edges[i].dest;
        int weight = edges[i].weight;

        // allocate new node of adjacency list from `src` to `dest`
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->dest = dest;
        newNode->weight = weight;

        // point new node to current head
        newNode->next = graph->head[src];

        // point head pointer to a new node
        graph->head[src] = newNode;

        // allocate new node of adjacency list from `dest` to `src`
        newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->dest = src;
        newNode->weight = weight;

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
    for (i = 0; i < N; i++)
    {
        // print the current vertex and all its neighbors
        struct Node* ptr = graph->head[i];
        while (ptr != NULL)
        {
            printf("%d -> %d (%d)\t", i, ptr->dest, ptr->weight);
            ptr = ptr->next;
        }

        printf("\n");
    }
}

// Weighted Directed Graph Implementation in C



/*
int main(void)
{
    // input array containing edges of the graph (as per above diagram)
    // `(x, y, w)` tuple in the array represents an edge from `x` to `y`
    // having weight `w`
    struct Edge edges[] =
            {
                    { 4, 1, 6 }, { 1, 2, 7 }, { 2, 0, 5 }, { 2, 1, 4 },
                    { 3, 2, 10 }, { 4, 5, 1 }, { 5, 4, 3 }
            };

    // calculate the number of edges
    int n = sizeof(edges)/sizeof(edges[0]);

    // construct a graph from the given edges
    struct Graph *graph = createGraph(edges, n);

    // print adjacency list representation of a graph
    printGraph(graph);

    return 0;
}
 */

// Function that constructs the edges for the graph
struct Edge* build_edges(station_list_node* list, route* r, int route_count, train* t){
    struct Edge* edges = malloc(sizeof(struct Edge) * route_count);

    for (int i = 0; i < route_count; ++i) {
        edges[i].src = search_station_list(list, r[i].station_start);
        edges[i].dest = search_station_list(list, r[i].station_end);
        edges[i].weight = weight_calc(r[i], *t);
    }

    return edges;
}