#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stations.h"
#include "trains.h"
#include "graph.h"

#define METER_CONVERSION 1000
#define METER_PER_SECOND_CONVERSION 3.6

#define DATA_SIZE 40
#define ROUTE_COUNT 3
#define TRAIN_COUNT 3
typedef struct route{
    int distance;
    int track_speed;
    char power[DATA_SIZE];
    char gauge[DATA_SIZE];
    char controls[DATA_SIZE];
    char station_start[DATA_SIZE];
    char station_end[DATA_SIZE];
} route;

typedef struct train{
    char name[DATA_SIZE];
    char gauge[DATA_SIZE];
    char controls[DATA_SIZE];
    char fuels[DATA_SIZE];
    double acceleration;
    double max_speed;
    int passenger_space;
} train;


double time(route r, train t);


double added_delay(route r, train t);

// First route data, then train data then everything else
int weight_calc(route r, train t);

void scan_routes(FILE*, route*);
void scan_trains(FILE*, train*);

int main(void)
{
    route routes[ROUTE_COUNT];
    FILE* routefile = fopen("..\\..\\src\\rutedata.txt", "r");

    scan_routes(routefile, routes);

    fclose(routefile);

    train trains[TRAIN_COUNT];
    FILE* trainfile = fopen("..\\..\\src\\trains.txt", "r");

    scan_trains(trainfile, trains);

    fclose(trainfile);

    double travel_time = time(routes[0], trains[0]);
    printf("\nTravel time from %s to %s - assuming stuff, not accurate: %lf s\n",routes[0].station_start, routes[0].station_end, travel_time);
    printf("Distance: %d km\n", station_distance(Berlin_Hbf, Paris_Nord));
    printf("Weight for above-mentioned route: %d\n", weight_calc(routes[0], trains[0]));


    // input array containing edges of the graph (as per above diagram)
    // `(x, y, w)` tuple in the array represents an edge from `x` to `y`
    // having weight `w`
    struct Edge edges[] =
            {
                    { Berlin_Hbf, Paris_Nord, weight_calc(routes[0], trains[0]) },
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


void scan_routes(FILE* p_file, route* r){   //reads from a file where each line has the following format: Distance,Track_speed,Power,Gauge,Controls,Station_Start,Station_End
    char temp[DATA_SIZE];
    for (int i = 0; i < ROUTE_COUNT; ++i) {

        fscanf(p_file, "%[^,]", temp);
        r[i].distance = strtol(temp, NULL, 10);  //using strtol here to convert the string we've read into a long integer
        fscanf(p_file, "%*[,]");
        fscanf(p_file, "%[^,]", temp);
        r[i].track_speed = strtol(temp, NULL, 10);
        fscanf(p_file, "%*[,]");
        fscanf(p_file, "%[^,]", r[i].power);
        fscanf(p_file, "%*[,]");
        fscanf(p_file, "%[^,]", r[i].gauge);        //read until a comma is found
        fscanf(p_file, "%*[,]");                    //dump the comma
        fscanf(p_file, "%[^,]", r[i].controls);     //read until the next comma
        fscanf(p_file, "%*[,]");                    //and so on
        fscanf(p_file, "%[^,]", r[i].station_start);
        fscanf(p_file, "%*[,]");
        fscanf(p_file, "%[^\n]", r[i].station_end); //station_end is the last piece of data. a newline character comes right after it
        fscanf(p_file, "%*[^a-zA-Z0-9]");           //skip characters until a letter or number is reached, which would be at the next line
    }
    //printf to test if the format is right. delete later
    printf("Routedata:\n%d %d %s %s %s %s %s\n",r[0].distance,r[0].track_speed,r[0].power,r[0].gauge,r[0].controls,r[0].station_start,r[0].station_end);
}

void scan_trains(FILE* p_file, train* t){   //reads from a file where each line has the following format: Name,Gauge,Control1.Control2.Control3,Fuel1.Fuel2.Fuel3,Acceleration,Max_speed,Passenger_space
    char temp[DATA_SIZE];
    for (int i = 0; i < TRAIN_COUNT; ++i) {

        fscanf(p_file, "%[^,]", t[i].name);
        fscanf(p_file, "%*[,]");
        fscanf(p_file, "%[^,]", t[i].gauge);
        fscanf(p_file, "%*[,]");
        fscanf(p_file, "%[^,]", t[i].controls);
        fscanf(p_file, "%*[,]");
        fscanf(p_file, "%[^,]", t[i].fuels);
        fscanf(p_file, "%*[,]");
        fscanf(p_file, "%[^,]", temp);
        t[i].acceleration = strtod(temp, NULL);        //converting the string to a double, so we can do math on it
        fscanf(p_file, "%*[,]");
        fscanf(p_file, "%[^,]", temp);
        t[i].max_speed = strtod(temp, NULL);
        fscanf(p_file, "%*[,]");
        fscanf(p_file, "%[^\n]", temp);
        t[i].passenger_space = strtol(temp, NULL, 10);
        fscanf(p_file, "%*[^a-zA-Z0-9]");

        //printf to test if the format is right. delete later
        printf("Traindata:\n%s %s %s %s %.0lf %.0lf %d\n",t[i].name,t[i].gauge,t[i].controls,t[i].fuels,t[i].acceleration,t[i].max_speed,t[i].passenger_space);
    }

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
