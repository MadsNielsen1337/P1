#include <stdio.h>
#include <stdlib.h>
#include "routes.h"
#include "trains.h"
#include "graph.h"
#include "UI.h"
#include "readFiles.h"
#include "string.h"

// Source vs header
// https://stackoverflow.com/questions/3482948/what-is-the-fundamental-difference-between-source-and-header-files-in-c

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
    struct Graph *graph = createGraph(edges, route_count);

    // For finding a random train
    printf("Random train is: %s\n", select_random_train(graph->head[2]->allowed_trains));

    // For finding all trains and comparing them to a string
    char train_test[DATA_SIZE];
    for (int i = 0; i <= select_all_trains_helper(graph->head[2]->allowed_trains); ++i) {
        strcpy(train_test, select_all_trains(graph->head[2]->allowed_trains, i));
        if(segmented_string_compare(train_test, "ICE1")) {
            printf("\nTrain %d is %s and matches ICE1\n", i, train_test);
        }
    }


    train_match(graph, 0, list_of_stations);


    // Draw the UI
    GenerateUI(routes, list_of_stations, graph);

    // Test to see if calc were done correct. Should be deleted later
    //printf("R = %d\n", route_count);
    //printf("T = %d\n", train_count);


    // Error-testing here. Should probably be moved or deleted later
    /*double travel_time = time(routes[0], trains[0]);
    printf("\nTravel time from %s to %s - assuming stuff, not accurate: %lf s\n",routes[1].station_start, routes[1].station_end, travel_time);
    printf("Distance: %d km\n", routes[1].distance);
    printf("Weight for above-mentioned route: %d\n", weight_calc(routes[0], trains[0]));
    *///list_test();


    //printf("\n%s", compatible_trains(trains, routes[0], train_count));

    //printf("Function thinks shortest route has weight %d\n", dijkstra(graph, 0, 10));

    // Give memory back to the OS
    free(edges);

    return EXIT_SUCCESS;
}


