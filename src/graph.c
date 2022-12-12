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
 * @param dist is the array which the function will write the distances into. The index to a given distance corresponds to the node that is reached in that distance. It should be as long as the number nodes in the graph.
 * @param prev is an array which the function will write the previous node to the node represented by the index value into. Its size should match @param dist
 * @param start_node is the integer corresponding to the node from which to measure distances from.
 * @param node_count is the number of nodes in the graph. This corresponds to the length of the linked station list.
 */
void dijkstra(struct Graph* graph, float* dist, int* prev, int start_node, int node_count) {
    for (int i = 0; i <
                    node_count; ++i) {  //setup initial distances to all vertices i. these are infinite to signal that they have not been visited yet
        dist[i] = INFINITY;
        prev[i] = -1;                   //no paths exist yet, so the previous node to any other is nonexistent, represented by -1
    }
    dist[start_node] = 0;               //we begin at this node, so the distance is 0
    prev[start_node] = -1;              //and there is no previous node to the first one, so we set it to -1


    float queue[node_count];

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

int train_match(struct Graph* graph, int search_pos, station_list_node* list_of_stations)
{
    if(search_pos < 0) {
        printf("\nError: Search_pos out of range with %d\n", search_pos);
        return 0;
    }

    // Get a random train for the initial node
    char first_train[DATA_SIZE];
    strcpy(first_train, select_random_train(graph->head[search_pos]->allowed_trains));


    // Check if the initial train is compatible with the next node
    // Currently has a problem if the selected random first_train is not compatible with next_train but another first_train would have been
    int i = 0, compat;
    char next_train[DATA_SIZE];
    for (i; i <= select_all_trains_helper(graph->head[search_pos]->next->allowed_trains); ++i) {
        strcpy(next_train, select_all_trains(graph->head[search_pos]->next->allowed_trains, i));
        if(segmented_string_compare(next_train, first_train)) {
            printf("\nPrevious train %d is %s (route [%d] %s) and matches next train %s (route [%d] %s)\n", i, next_train, search_pos,
                   index_station_list(list_of_stations, search_pos), first_train, graph->head[search_pos]->next->dest,
                   index_station_list(list_of_stations, graph->head[search_pos]->dest));
            return 1;
        }
        printf("test %d\n", i);
    }

    printf("\nPrevious train %d is %s (route [%d] %s) and DOES NOT match next train %s (route [%d] %s)\n", i,
           next_train, search_pos,
           index_station_list(list_of_stations, search_pos), first_train, graph->head[search_pos]->next->dest,
           index_station_list(list_of_stations, graph->head[search_pos]->next->dest));
    return 0;



    // For finding a random train
    //printf("Random train is: %s\n", select_random_train(graph->head[search_pos]->allowed_trains));

    /*
    // For finding all trains and comparing them to a string
    char train_test[DATA_SIZE];
    for (int i = 0; i <= select_all_trains_helper(graph->head[2]->allowed_trains); ++i) {
        strcpy(train_test, select_all_trains(graph->head[2]->allowed_trains, i));
        if(segmented_string_compare(train_test, "ICE1")) {
            printf("\nTrain %d is %s and matches ICE1\n", i, train_test);
        }
    }
    */

}



// =========================================
// Code scrap of another iteration of dijkstra
/*
    float queue[node_count];

    for (int i = 0; i < node_count; ++i) {
        for (int j = 0; j < node_count; ++j) {
            if(queue[j] != INFINITY){
                queue[j] = dist[j];
            }
        }

        float temp = INFINITY;
        int index;
        for (int j = 0; j < node_count; ++j) {
            if(queue[j] < temp){
                temp = queue[j];
                index = j;
                printf("\nIndex updated to %d\n", j);
            }
        }
        queue[index] = INFINITY;
*/

// =========================================
// The old dijkstra-based algorithm

//graph->head[search_pos]->weight + dijkstra_recursive(graph, graph->head[search_pos]->dest, target, total_weight, search_pos);
//code scrap above searches the first available destination
//helper function for dijkstra_recursive to check if an integer representing a node in the graph is in an array which represents the steps already taken
/*int visited_position(const int target, const int* positions){
    for (int i = 0; i < N; ++i) {
        if(positions[i] == target){
            return 1;                //returns true if the target integer is somewhere in the positions-array
        }
    }
    return 0;   //if the for-loop finishes, we know the target is not in the positions-array and return false
}

// Recursive function that searches the adjacency list generated by createGraph until it the target node matches the node being searched and returns the smallest weight needed to get there
int dijkstra_recursive(struct Graph* graph, int search_pos, int target, int total_weight, int iteration_nr, int* path){
    if (visited_position(search_pos, path)) {//detect if the algorithm tries to go back, meaning it has hit a dead end
        printf("\nDEAD END AT %d\n", search_pos);
        return -(total_weight + 1);
    } else {
        if(search_pos == target) {                         //check if we have reached the target and return 0 if we have, stopping the recursion
            printf("\nFOUND TARGET %d\n", search_pos);
            return 0;
        } else {
            printf("\nTRIED %d\n", search_pos);     //call the function on all destinations reachable from search_pos

            int new_path[N];                              //create a new path array and copy the previous path into it
            for (int i = 0; i < N; ++i) {
                new_path[i] = path[i];
            }
            new_path[iteration_nr] = search_pos;          //place the current position at the end of the path

            struct Node* ptr = graph->head[search_pos];
            int weight = -1;
            int temp;
            while (ptr != NULL){
                temp = dijkstra_recursive(graph, ptr->dest, target, total_weight, iteration_nr + 1, new_path);

                if(temp != -1){                            //check if we have found an existing route with a smaller weight and replace the weight if we have
                    temp = graph->head[search_pos]->weight + temp;
                    if(temp < weight || weight == -1){
                        weight = temp;
                    }
                }
                ptr = ptr->next;                           //get ready to go to the next destination reachable from search_pos
            }
            return weight;
        }
    }
}

//graph->head[search_pos]->weight + dijkstra_recursive(graph, graph->head[search_pos]->dest, target, total_weight, search_pos);
//code scrap above searches the first available destination

// Helper function for dijkstra_recursive that calls it with the weight-variable set as it should be and keeps track of the number of recursions to print out the order locations were checked in
int dijkstra(struct Graph* graph, int start_node, int end_node)
{
    int path[N];                    //create an array to hold the path the pathfinding-function takes
    for (int i = 0; i < N; ++i) {
        path[i] = -1;
    }
    int dijkstra_result = dijkstra_recursive(graph, start_node, end_node, 0, 0, path);
    printf("Path taken is");
    for (int i = 0; i < N; ++i) {
        if(path[i] != -2) {                     //remember to set -2 to -1 again
            printf(" %d\n", path[i]);
        }
    }
    return dijkstra_result;
}*/