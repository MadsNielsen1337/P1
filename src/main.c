#include <stdio.h>
#include <stdlib.h>
#include "routes.h"
#include "trains.h"
#include "graph.h"
#include "UI.h"
#include "readFiles.h"
#include "time_calc.h"
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
    struct Graph* graph = createGraph(edges, route_count);

    // For finding a random train
    //printf("Random train is: %s\n", select_random_train(graph->head[2]->allowed_trains));

    // For finding all trains and comparing them to a string
    /*char train_test[DATA_SIZE];
    for (int i = 0; i <= select_all_trains_helper(graph->head[2]->allowed_trains); ++i) {
        strcpy(train_test, select_all_trains(graph->head[2]->allowed_trains, i));
        if(segmented_string_compare(train_test, "ICE1")) {
            printf("\nTrain %d is %s and matches ICE1\n", i, train_test);
        }
    }*/

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
    int node_count = (list_length(list_of_stations) - 1);

    float dist[node_count];    //holds the distances from the start_node to every other node in the graph
    int prev[node_count];    //holds the previous node in the shortest path to the node corresponding to the index
    float new_dist[node_count];
    float average[node_count];
    float percent[node_count];
    char start_train[DATA_SIZE];
    for (int i = 0; i < node_count; ++i) {
        printf("\nDIJKSTRA %d", i);

        dijkstra(graph, dist, prev, i, list_length(list_of_stations));
        for (int j = 0; j < node_count; ++j) {
            //printf("\n%f + %f", dist[j],(float)delay_optimised(graph, start_train , prev, i, j ));
        }

        for (int j = 0; j < node_count; ++j) {
            struct Node *ptr = graph->head[j];
            while (ptr != NULL && ptr->dest != prev[j]) {     //find the correct route to check for matching trains on
                ptr = ptr->next;
            }
            if(i != j) {
                new_dist[j] = dist[j] + (float) delay_optimised(graph, ptr->allowed_trains, prev, i, j);
                new_dist[i] = 0;
                //printf("\n[%d] Delay optimised: %lf\n", j, (float)delay_optimised(graph, ptr->allowed_trains, prev, i, j));
                //printf("[%d] Dist: %f\n", j, dist[j]);
                //printf("\n[%d] New dist: %f\n", j, new_dist[j]);
                /*
                if(900 < (float) delay_optimised(graph, ptr->allowed_trains, prev, i, j)){
                    printf("Starting with: %s", ptr->allowed_trains);
                    printf("\nOptimised: %lf\n", (float) delay_optimised(graph, ptr->allowed_trains, prev, i, j));
                    printf("Not Optimised: %lf\n", (float) extra_delay(graph, j, i, prev, start_train));
                }
                 */
            }


        }
        average[i] = average_weight_difference(new_dist, dist, node_count);
        percent[i] =  percentage_weight_difference(new_dist, dist, node_count);
        printf("\n[%d] Average delay is %f", i, average_weight_difference(new_dist, dist, node_count));
        printf("\n[%d] Average extra time in percent %f", i,  percent[i]);
        printf("\n\n");
    }
    printf("\nAverage og averages: %lf\n", average_simple(average, node_count));
    printf("Average of percent: %lf\n", average_simple(percent, node_count));

    // Draw the UI - NO FUNCTION THAT NEED EXECUTION MAY BE PLACED BELOW THE UI
    GenerateUI(routes, list_of_stations, graph, dist, prev);

    // Give memory back to the OS
    free(edges);

    return EXIT_SUCCESS;
}


