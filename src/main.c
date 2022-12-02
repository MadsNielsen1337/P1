#include <stdio.h>
#include <stdlib.h>
#include "routes.h"
#include "trains.h"
#include "graph.h"
#include "UI.h"
#include "readFiles.h"

// Source vs header
// https://stackoverflow.com/questions/3482948/what-is-the-fundamental-difference-between-source-and-header-files-in-c

#define METER_CONVERSION 1000
#define METER_PER_SECOND_CONVERSION 3.6

double time(route r, train t);
double added_delay(route r, train t);
int weight_calc(route r, train t);

int main(void)
{
    int route_count = 0;
    int train_count = 0;

    route_count = get_route_size(route_count);  // Calculate size of route.txt - aka. how many lines are there?
    train_count = get_train_size(train_count);  // Calculate size of trains.txt - aka. how many lines are there?

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

double time(route r, train t)
{
    if(r.track_speed < t.max_speed) {
        t.max_speed = r.track_speed;
    }

    double time_max_speed;
    t.max_speed /= METER_PER_SECOND_CONVERSION;
    r.distance *= METER_CONVERSION;

    // Since data about decel is hard to find, it is expected that accel = decel.
    // This accounts both for accel and decel
    time_max_speed = 2 * ((t.max_speed) / t.acceleration); // Unit: s

    // Accel
    // v = a * t + v_0

    // t = (v-v_0)/a

    // t = (sqrt(2 * a_0 * s - 2 * a_0 * s_0 + v_0^2) - v_0)/ a_0

    double time = (r.distance) / t.max_speed; // Unit: s

    // This simple solution subtracts the accel/decel time with total time. While it isn't 100% accurate it tries to make an estimate of the time taken to travel the given distance.
    double total_time = (time - time_max_speed) + added_delay(r, t);

    return total_time;
}

double added_delay(route r, train t)
{
    double delay = 0;    // Delay time in minutes

    if(t.fuels != r.power)
        delay += 5;
    if(t.gauge != r.gauge)
        delay += 5;
    if(t.controls != r.controls)
        delay += 5;

    return delay;

}

int weight_calc(route r, train t)
{
    int weight = 0;    // Delay time in minutes

    weight += time(r, t);
    // maybe also make a delay at each vertex to account for passengers leaving/getting on the train - unless the vertex is the final destination


    // Comments down below are loose thoughts and random stuff


    // We convert km && km/h to m && m/s
    //   weight += ((distance * METER_CONVERSION) / (avg_track_speed_r/METER_PER_SECOND_CONVERSION)); // distance travel time with average track speed.

    // check to see if the train in use can continue onto the next edge. Find a way to check layer 3 & 4 stats for next edge.
    /*if(power_f != fuel_type_s || track_gauge_f != train_track_gauge_s || control_f != control_s)             // Doubt you can do this. Prolly need strcmp or something
        weight += 10;       // Added delay in minutes. Find the average time it takes to switch trains.
*/

    return weight;
}
