#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stations.h"
#include "trains.h"
#include "graph.h"

double time(double accel_ts,
            double max_speed_ts,
            double distance,
            int track_speed,
            char track_gauge_ts[],
            char control_ts[],
            char fuel_type_ts[],
            char track_gauge[],
            char power[],
            char control[]);

double added_delay(char track_gauge_ts[],
                    char control_ts[],
                    char fuel_type_ts[],
                    char track_gauge[],
                    char power[],
                    char control[]);


int main(void)
{
    printf("%s\n",IC4.name);

    double travel_time = time(IC4.accel,
                              IC4.max_speed,
                              station_distance(Berlin_Hbf, Paris_Nord),
                              9999,
                              IC4.track_gauge,
                              IC4.control,
                              IC4.fuel_type,
                              "Standard",
                              "Fisk",
                              "Fisk2");
    printf("Travel time from berlinHBF to parisNord - assuming stuff, not accurate: %lf s\n", travel_time);
    printf("Distance: %.2lf m\n", station_distance(Berlin_Hbf, Paris_Nord));

    // input array containing edges of the graph (as per above diagram)
    // `(x, y, w)` tuple in the array represents an edge from `x` to `y`
    // having weight `w`
    struct Edge edges[] =
            {
                    { Berlin_Hbf, Paris_Nord, 1000 },
                    { 1, 2, 2 },
                    { 2, 0, 3 },
                    { 2, 1, 4 },
                    { 3, 2, 5 },
                    { 4, 5, 6 },
                    { 5, 4, 7 }
            };

    // calculate the number of edges
    int n = sizeof(edges)/sizeof(edges[0]);

    // construct a graph from the given edges
    struct Graph *graph = createGraph(edges, n);

    // print adjacency list representation of a graph
    printGraph(graph);


    return EXIT_SUCCESS;
}

double time(double accel_ts,
            double max_speed_ts,
            double distance,
            int track_speed,
            char track_gauge_ts[],
            char control_ts[],
            char fuel_type_ts[],
            char track_gauge[],
            char power[],
            char control[])
{
    if(track_speed < max_speed_ts) {
        max_speed_ts = track_speed;
    }

    double time_max_speed = 0;

    // Since data about decel is hard to find, it is expected that accel = decel.

    // This accounts both for accel and decel
    time_max_speed = 2 * (max_speed_ts/accel_ts); // Unit: s

    // Accel
    // v = a * t + v_0

    // t = (v-v_0)/a

    // t = (sqrt(2 * a_0 * s - 2 * a_0 * s_0 + v_0^2) - v_0)/ a_0

    double time = distance/max_speed_ts; // Unit: s

    // This simple solution subtracts the accel/decel time with total time. While it isn't 100% accurate it tries to make an estimate of the time taken to travel the given distance.
    double total_time = (time - time_max_speed) + (60 * added_delay(track_gauge_ts, control_ts, fuel_type_ts, track_gauge, power, control));

    return total_time;
}

double added_delay(char track_gauge_ts[],
                   char control_ts[],
                   char fuel_type_ts[],
                   char track_gauge[],
                   char power[],
                   char control[])
{
    double delay = 0; // Delay time in minutes

    if(fuel_type_ts != power)
        delay += 5;
    if(track_gauge_ts != track_gauge)
        delay += 5;
    if(control_ts != control)
        delay += 5;

    return delay;

}

