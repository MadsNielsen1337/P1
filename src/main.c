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


double time(double train_accel,
            double train_max_speed,
            double distance,
            int track_speed,
            char track_gauge_ts[],
            char control_ts[],
            char fuel_type_ts[],
            char track_gauge[],
            char power[],
            char control[]);

double added_delay(char track_gauge_ts[], char control_ts[], char fuel_type_ts[], char track_gauge[], char power[], char control[]);

int weight_calc(int avg_track_speed, int distance, char power_f[], char track_gauge_f[], char control_f[], char train_in_use_f[],
                char train_type_s[], char train_track_gauge_s[], char control_s[], char fuel_type_s[], double max_speed);

void scan_routes(FILE*, route*);
void scan_trains(FILE*, train*);

int main(void)
{
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
    printf("Distance: %d km\n", station_distance(Berlin_Hbf, Paris_Nord));
    printf("Weight for above-mentioned route: %d\n", weight_calc(230, station_distance(Berlin_Hbf, Paris_Nord),
                                                                 "15kV_16Hz", "Standard", "ETCS.LZB.PZB", "IC4", IC4.name,
                                                                 IC4.track_gauge, IC4.control, IC4.fuel_type, IC4.max_speed));


    // input array containing edges of the graph (as per above diagram)
    // `(x, y, w)` tuple in the array represents an edge from `x` to `y`
    // having weight `w`
    struct Edge edges[] =
            {
                    { Berlin_Hbf, Paris_Nord, weight_calc(160, station_distance(Berlin_Hbf, Paris_Nord),
                                                          "15kV_16Hz", "Standard", "ETCS,LZB,PZB", "IC4", IC4.name,
                                                          IC4.track_gauge, IC4.control, IC4.fuel_type, IC4.max_speed) },
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



    route routes[ROUTE_COUNT];
    FILE* routefile = fopen("..\\..\\src\\rutedata.txt", "r");

    scan_routes(routefile, routes);

    fclose(routefile);

    train trains[TRAIN_COUNT];
    FILE* trainfile = fopen("..\\..\\src\\trains.txt", "r");

    scan_trains(trainfile, trains);

    fclose(trainfile);

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
    printf("\n%d %d %s %s %s %s %s\n",r[0].distance,r[0].track_speed,r[0].power,r[0].gauge,r[0].controls,r[0].station_start,r[0].station_end);
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
    }
    //printf to test if the format is right. delete later
    printf("\n%s %s %s %s %lf %lf %d\n",t[0].name,t[0].gauge,t[0].controls,t[0].fuels,t[0].acceleration,t[0].max_speed,t[0].passenger_space);
}



double time(double train_accel,
            double train_max_speed,
            double distance,
            int track_speed,
            char track_gauge_ts[],
            char control_ts[],
            char fuel_type_ts[],
            char track_gauge[],
            char power[],
            char control[])
{
    if(track_speed < train_max_speed) {
        train_max_speed = track_speed;
    }

    double time_max_speed;
    train_max_speed /= METER_PER_SECOND_CONVERSION;
    distance *= METER_CONVERSION;

    // Since data about decel is hard to find, it is expected that accel = decel.
    // This accounts both for accel and decel
    time_max_speed = 2 * ((train_max_speed) / train_accel); // Unit: s

    // Accel
    // v = a * t + v_0

    // t = (v-v_0)/a

    // t = (sqrt(2 * a_0 * s - 2 * a_0 * s_0 + v_0^2) - v_0)/ a_0

    double time = (distance) / train_max_speed; // Unit: s

    // This simple solution subtracts the accel/decel time with total time. While it isn't 100% accurate it tries to make an estimate of the time taken to travel the given distance.
    double total_time = (time - time_max_speed) + added_delay(track_gauge_ts, control_ts, fuel_type_ts, track_gauge, power, control);

    return total_time;
}

double added_delay(char track_gauge_ts[],
                   char control_ts[],
                   char fuel_type_ts[],
                   char track_gauge[],
                   char power[],
                   char control[])
{
    double delay = 0;    // Delay time in minutes

    if(fuel_type_ts != power)
        delay += 5;
    if(track_gauge_ts != track_gauge)
        delay += 5;
    if(control_ts != control)
        delay += 5;

    return delay;

}

//track speed, distance, power, gauge, control and train in
int weight_calc(int avg_track_speed, int distance, char power_f[], char track_gauge_f[], char control_f[], char train_in_use_f[],
                char train_type_s[], char train_track_gauge_s[], char control_s[], char fuel_type_s[], double max_speed)
{
    int weight = 0;    // Delay time in minutes

    // We convert km && km/h to m && m/s
    //   weight += ((distance * METER_CONVERSION) / (avg_track_speed/METER_PER_SECOND_CONVERSION)); // distance travel time with average track speed.

    // check to see if the train in use can continue onto the next edge. Find a way to check layer 3 & 4 stats for next edge.
    /*if(power_f != fuel_type_s || track_gauge_f != train_track_gauge_s || control_f != control_s)             // Doubt you can do this. Prolly need strcmp or something
        weight += 10;       // Added delay in minutes. Find the average time it takes to switch trains.
*/
    weight += time(IC4.accel,
                   IC4.max_speed,
                   station_distance(Berlin_Hbf, Paris_Nord),
                   9999,
                   IC4.track_gauge,
                   IC4.control,
                   IC4.fuel_type,
                   "Standard",
                   "Fisk",
                   "Fisk2");
    // maybe also make a delay at each vertex to account for passengers leaving/getting on the train - unless the vertex is the final destination

    return weight;
}
