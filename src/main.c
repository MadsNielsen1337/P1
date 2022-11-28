#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stations.h"
#include "trains.h"
#include "graph.h"

double time(double train_accel, double max_speed, double distance, int track_speed);

int main(void)
{
    printf("%s\n",IC4.name);

    double travel_time = time(IC4.accel, IC4.max_speed, berlinHBF_parisNord, 9999);
    printf("Travel time from berlinHBF to parisNord - assuming stuff, not accurate: %lf s\n", travel_time);


    // input array containing edges of the graph (as per above diagram)
    // `(x, y, w)` tuple in the array represents an edge from `x` to `y`
    // having weight `w`
    struct Edge edges[] =
            {
                    { Berlin_Hbf, Paris_Nord, 6 }, { 1, 2, 7 }, { 2, 0, 5 }, { 2, 1, 4 },
                    { 3, 2, 10 }, { 4, 5, 1 }, { 5, 4, 3 }
            };

    // calculate the number of edges
    int n = sizeof(edges)/sizeof(edges[0]);

    // construct a graph from the given edges
    struct Graph *graph = createGraph(edges, n);

    // print adjacency list representation of a graph
    printGraph(graph);






    return EXIT_SUCCESS;
}

double time(double accel, double max_speed, double distance, int track_speed)
{
    if(track_speed < max_speed) {
        max_speed = track_speed;
    }

    double time_max_speed = 0;

    // Since data about decel is hard to find, it is expected that accel = decel.

    // This accounts both for accel and decel
    time_max_speed = 2 * (max_speed/accel); // Unit: s

    // Accel
    // v = a * t + v_0

    // t = (v-v_0)/a

    // t = (sqrt(2 * a_0 * s - 2 * a_0 * s_0 + v_0^2) - v_0)/ a_0

    double time = distance/max_speed; // Unit: s

    // This simple solution subtracts the accel/decel time with total time. While it isn't 100% accurate it tries to make an estimate of the time taken to travel the given distance.
    double total_time = time - time_max_speed;

    return total_time;
}



