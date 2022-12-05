#include <stdio.h>
#include <stdlib.h>
#include "routes.h"
#include "trains.h"
#include "graph.h"
#include "UI.h"
#include "readFiles.h"
#include "time_calc.h"

// Source vs header
// https://stackoverflow.com/questions/3482948/what-is-the-fundamental-difference-between-source-and-header-files-in-c

int main(void)
{
    int route_count = 0;
    int train_count = 0;

    FILE* routefile = fopen("..\\..\\src\\rutedata.txt", "r");
    route_count = lines_in_file(routefile);  // Calculate size of route.txt - aka. how many lines are there?
    fclose(routefile);

    FILE* trainfile = fopen("..\\..\\src\\trains.txt", "r");
    train_count = lines_in_file(trainfile);  // Calculate size of trains.txt - aka. how many lines are there?
    fclose(trainfile);

    // Test to see if calc were done correct. Should be deleted later
    printf("R = %d\n", route_count);
    printf("T = %d\n", train_count);

    // Allocate space in the arrays for input size of *.txt files
    train trains[train_count];
    route routes[route_count];

    // Read data from *.txt files and assign to corresponding place in the array
    read_routes(routes, &route_count);
    read_trains(trains, &train_count);

    // Build the station list from the stations in the route array
    station_list_node* list_of_stations = build_station_list(routes, &route_count);

    // Error-testing here. Should probably be moved or deleted later
    double travel_time = time(routes[0], trains[0]);
    printf("\nTravel time from %s to %s - assuming stuff, not accurate: %lf s\n",routes[1].station_start, routes[1].station_end, travel_time);
    printf("Distance: %d km\n", routes[1].distance);
    printf("Weight for above-mentioned route: %d\n", weight_calc(routes[0], trains[0]));
    //list_test();

    // Input array containing edges of the graph
    struct Edge* edges = build_edges(list_of_stations, routes, route_count, trains);

    // Construct a graph from the given edges
    struct Graph *graph = createGraph(edges, route_count);





    // Print adjacency list representation of a graph
    printGraph(graph);

    // Draw the UI
    GenerateUI(routes, list_of_stations);

    // Give memory back to the OS
    free(edges);

    return EXIT_SUCCESS;
}


