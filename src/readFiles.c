#include <stdio.h>
#include <stdlib.h>
#include "routes.h"
#include "trains.h"
#include "readFiles.h"

// Counts the number of lines in the file pointed to by p_file. Uses up the entire input stream, so if the file needs to be used again it must be closed and reopened!
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
        while (c != EOF);           //repeats until the end of the file and returns the number of newline characters
    }
    return count;
}

// Reads input in route.txt using scan_routes
void read_routes(route* routes, int* route_count)
{
    FILE* routefile = fopen("routes.txt", "r");
    scan_routes(routefile, routes, route_count);
    fclose(routefile);
}

// // Reads input in trains.txt using scan_trains
void read_trains(train* trains, int* train_count)
{
    FILE* trainfile = fopen("trains.txt", "r");
    scan_trains(trainfile, trains, train_count);
    fclose(trainfile);
}

// Reads from a file where each line has the following format: Distance,Track_speed,Power,Gauge,Controls,Station_Start,Station_End
void scan_routes(FILE* p_file, route* r, const int* route_count)
{
    char temp[DATA_SIZE];
    for (int i = 0; i < *route_count; ++i) {

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
}

// Reads from a file where each line has the following format: Name,Track_gauge,Control1.Control2.Control3,Fuel1.Fuel2.Fuel3,Acceleration,Max_speed,Passenger_cap
void scan_trains(FILE* p_file, train* t, const int* train_count)
{
    char temp[DATA_SIZE];
    for (int i = 0; i < *train_count; ++i) {

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
}
