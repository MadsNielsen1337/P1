#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "routes.h"
#include "trains.h"
#include "graph.h"

#define METER_CONVERSION 1000
#define METER_PER_SECOND_CONVERSION 3.6
#define DATA_SIZE 40



double time(route r, train t);


double added_delay(route r, train t);

// First route data, then train data then everything else
int weight_calc(route r, train t);

int lines_in_file(FILE*);

int main(void)
{
    int route_count = 0, train_count = 0;

    FILE* routefile = fopen("..\\..\\src\\rutedata.txt", "r");
    route_count = lines_in_file(routefile);
    fclose(routefile);

    route routes[route_count];

    routefile = fopen("..\\..\\src\\rutedata.txt", "r");
    scan_routes(routefile, routes);
    fclose(routefile);

    FILE* trainfile = fopen("..\\..\\src\\trains.txt", "r");
    train_count = lines_in_file(trainfile);
    fclose(trainfile);

    train trains[train_count];

    trainfile = fopen("..\\..\\src\\trains.txt", "r");
    scan_trains(trainfile, trains);
    fclose(trainfile);

    station_list_node* list_of_stations = build_station_list(routes); //build the station list from the stations in the route array

    //error-testing here. Should probably be moved or deleted later
    double travel_time = time(routes[0], trains[0]);
    printf("\nTravel time from %s to %s - assuming stuff, not accurate: %lf s\n",routes[0].station_start, routes[0].station_end, travel_time);
    printf("Weight for above-mentioned route: %d\n", weight_calc(routes[0], trains[0]));


    // input array containing edges of the graph
    struct Edge* edges = build_edges(list_of_stations, routes, route_count, trains);

    /* calculate the number of edges
    int n = sizeof(edges)/sizeof(edges[0]);     Deprecated since route_count should have the same value and can be used instead
    */

    // construct a graph from the given edges
    struct Graph *graph = createGraph(edges, route_count);

    // print adjacency list representation of a graph
    printGraph(graph);

    free(edges);


    return EXIT_SUCCESS;
}


//int distance, int avg_track_speed_r, char power_r[], char track_gauge_r[], char control_r[],
//            double max_speed_t, char fuel_type_t[], char track_gauge_t[], char control_t[], double train_accel_t
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


//char power_r[], char track_gauge_r[], char control_r[],
//                   char fuel_type_t[], char track_gauge_t[], char control_t[]
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
//char station_start[], char station_end[], int distance, int avg_track_speed_r, char power_r[], char track_gauge_r[], char control_r[],
//                char train_name[], double max_speed_t, char fuel_type_t[], char track_gauge_t[], char control_t[], double train_accel_t
int weight_calc(route r, train t)
{
    int weight = 0;    // Delay time in minutes

    // We convert km && km/h to m && m/s
    //   weight += ((distance * METER_CONVERSION) / (avg_track_speed_r/METER_PER_SECOND_CONVERSION)); // distance travel time with average track speed.

    // check to see if the train in use can continue onto the next edge. Find a way to check layer 3 & 4 stats for next edge.
    /*if(power_f != fuel_type_s || track_gauge_f != train_track_gauge_s || control_f != control_s)             // Doubt you can do this. Prolly need strcmp or something
        weight += 10;       // Added delay in minutes. Find the average time it takes to switch trains.
*/
    weight += time(r, t);
    // maybe also make a delay at each vertex to account for passengers leaving/getting on the train - unless the vertex is the final destination

    return weight;
}

//counts the number of lines in the file pointed to by p_file. Uses up the entire input stream, so if the file needs to be used again it must be closed and reopened!
int lines_in_file(FILE* p_file){
    int count = 0;
    int c;
    if(p_file != NULL){             //check if the file exists before we start trying to read it. set count to 1 if it does exist since there will always be at least one line
        count = 1;
        do {                        //grabs the next character in the file and checks if it's a newline character
            c = getc(p_file);
            if(c == '\n'){
                count++;
            }
        }
        while (c != EOF);   //repeats until the end of the file and returns the number of newline characters
    }
    return count;
}