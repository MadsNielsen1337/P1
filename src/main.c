#include <stdio.h>
#include <stdlib.h>
#include "routes.h"
#include "trains.h"
#include "graph.h"
#include "UI.h"
#include "readFiles.h"

int main(void)
{
    int route_count = 0;
    int train_count = 0;

    // Calculate size of route.txt - aka. how many lines are there?
    FILE* routefile = fopen("..\\..\\src\\routes.txt", "r");
    route_count = lines_in_file(routefile);
    fclose(routefile);

    // Calculate size of trains.txt - aka. how many lines are there?
    FILE* trainfile = fopen("..\\..\\src\\trains.txt", "r");
    train_count = lines_in_file(trainfile);
    fclose(trainfile);

    // Allocate space in the arrays for input size of *.txt files
    train trains[train_count];
    route routes[route_count];

    // Read data from *.txt files and assign to corresponding place in the array
    read_routes(routes, &route_count);
    read_trains(trains, &train_count);


    // Build the station list from the stations in the route array
    station_list_node* list_of_stations = build_station_list(routes, &route_count);

    // Input array containing edges of the graph
    struct Edge* edges = build_edges(list_of_stations, routes, route_count, trains, train_count);

    // Construct a graph from the given edges
    struct Graph* graph = createGraph(edges, route_count);

    // Stuff used in dijkstra and its associated functions
    int node_count = (list_length(list_of_stations) - 1);
    float dist[node_count];    //holds the distances from the start_node to every other node in the graph
    int prev[node_count];      //holds the previous node in the shortest path to the node corresponding to the index
    float new_dist[node_count];
    float average[node_count];
    float percent[node_count];
    float delays[node_count];

    // Draw the UI - NO FUNCTION THAT NEED EXECUTION MAY BE PLACED BELOW THE UI
    GenerateUI(node_count, list_of_stations, graph, dist, prev, new_dist, average, percent, delays, trains, train_count);

    // Give memory back to the OS
    free(edges);

    return EXIT_SUCCESS;
}