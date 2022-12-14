#include "routes.h"
#include "trains.h"
#include "time_calc.h"
#include "graph.h"
#include <stdlib.h>
#include <string.h>
#include "stdio.h"

#define METER_CONVERSION 1000               // Multiply this with km to get m
#define METER_PER_SECOND_CONVERSION 3.6     // Divide this with km/h to get m/s
#define CHANGE_TIME 900                     // Time to change trains in seconds. Obviously an estimation

// Calculate the time it takes to go from station_start to station_end. Takes accel & decel into account
double time(route r, train t)
{
    if(r.track_speed < t.max_speed)
        t.max_speed = r.track_speed;


    double time_max_speed;
    t.max_speed /= METER_PER_SECOND_CONVERSION;
    r.distance *= METER_CONVERSION;

    // Since data about decel is hard to find, it is expected that accel = decel.
    // This accounts both for accel and decel

    // Accel
    // v = a * t + v_0

    // t = (v-v_0)/a

    // t = (sqrt(2 * a_0 * s - 2 * a_0 * s_0 + v_0^2) - v_0)/ a_0


    // The time it takes to accelerate from v = 0 to t.max_speed (Times 2 to account for decel)
    time_max_speed = 2 * ((t.max_speed) / t.acceleration); // Unit: s

    double v;
    // Velocity at time_max_speed
    v = t.acceleration * time_max_speed / 2;

    // The time it takes to travel distance with velocity v
    double time = (r.distance) / t.max_speed; // Unit: s

    // This simple solution subtracts the accel/decel time with total time. While it isn't 100% accurate it tries to make an estimate of the time taken to travel the given distance.
    double total_time = (time - time_max_speed) + added_delay(r, t);

    if(time_max_speed > total_time) {
        total_time = (time - (time_max_speed / 2)) + added_delay(r, t);
    }

    return total_time;
}

// Adds delay to the time if we're switching trains. More or less a placeholder for now
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

// Calculates the weight for the edge going from station_start to station_end
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

    if(weight < 0) {
        return -1;
    }

    return weight;
}

//copies a given index of a segmented string into an output string
void index_segmented_string(char* string, int index, char* output){
    int index_local = index;
    char string_local[XL_DATA_SIZE];
    strcpy(string_local, string); //creating a local version of the string to do work on

    for (int i = strlen(string_local) - 1; i >= 0; --i) {
        if (string_local[i] == '.' || i == 0) {
            if (index_local == 0 || i == 0) { //copying the string to the output once the index is 0 or the end of the segmented string is reached
                if (i != 0) {
                    strcpy(output, &string_local[i + 1]);
                } else {
                    strcpy(output, &string_local[i]);
                }
            }
            index_local--;                   //decreasing the index value every time a . is encountered
            string_local[i] = '\0';          //replacing the encountered . with an end character to be able to correctly read the next string
        }

    }
}

//get the number of entries in an existing segmented string. Used in find_best_train
int segmented_string_length(char* string){
    int count = 1;          //count initialised to 1 instead of 0 as the number of periods are equal to the number of entries minus 1
    if(string[0] == '\0'){  //check if the string is empty and return 0 if it is
        return 0;
    }
    for (int i = strlen(string) - 1; i >= 0; i--) {
        if (string[i] == '.') {
            count++;
        }
    }
    return count;
}

//helper function for extra_delay that identifies the train that is compatible with the route extra_delay is used on for the largest amount of stations, to minimise the amount of changes needed
char* find_best_train(const struct Graph* graph, char* chosen_trains, const int* prev, const int end_pos, const int pos) {
    struct Node *ptr = graph->head[pos];
    while (ptr != NULL && ptr->dest != prev[pos]) {     //find the correct route to check for matching trains on
        ptr = ptr->next;
    }

    char new_trains[XL_DATA_SIZE];  //initialise an array to hold matching trains and let the first spot be the end, so strcat can be used to build it
    new_trains[0] = '\0';
    char to_search[XL_DATA_SIZE];   //initialise an array to hold a copy of chosen trains, so we can do work on it
    strcpy(to_search, chosen_trains);
    char selected_train[XL_DATA_SIZE];

    for (int i = strlen(chosen_trains); i >= 0; --i) {
        if (to_search[i] == '.' || i == 0) {
            if(i != 0){
                to_search[i] = '\0';
                strcpy(selected_train, &to_search[i + 1]);
            } else {
                strcpy(selected_train, &to_search[i]);
            }
            if (segmented_string_compare(ptr->allowed_trains, selected_train)) {
                strcat(new_trains, selected_train);
                strcat(new_trains, ".");
            }
        }
    }

    int last_char = strlen(new_trains) - 1;               //format new segmented string correctly
    if (new_trains[last_char] == '.') {
        new_trains[last_char] = '\0';
    }

    if (segmented_string_length(new_trains) <= 1 || prev[pos] == end_pos) {       //check there are any matching trains i.e. entries in new_trains or if this is the last position where a change will happen
        char* best_train = malloc(sizeof(char) * DATA_SIZE);

        if (segmented_string_length(chosen_trains) > 1) {
            index_segmented_string(chosen_trains, 0, best_train);

            return best_train;
        } else {
            strcpy(best_train, chosen_trains);
            return best_train;                                                         //return the previously chosen trains if there are no matches in this iteration
        }
    } else {
        return find_best_train(graph, new_trains, prev, end_pos, prev[pos]);
    }
}

//this function finds the extra delay (as defined by CHANGE_TIME) added from travelling from pos to end_pos via the prev-array generated by the dijkstra function. Requires a train to be chosen before it is run. Note that end_pos is the position where the dijkstra-function begins its pathfinding.
int extra_delay(const struct Graph* graph, int pos, const int end_pos, const int* prev, char* chosen_train) {
    if (pos == end_pos) {
        return 0;                               //end the recursion once we reach the target position
    } else {
        int delay = 0;                          //base delay value. If there is no train change, this is the delay added by passing a station
        struct Node *ptr = graph->head[pos];    //setup pointer to adjacency list for the graph node "pos"
        while (ptr != NULL && ptr->dest != prev[pos]) {
            ptr = ptr->next;
        }
        if (!segmented_string_compare(ptr->allowed_trains, chosen_train)){       //add delay and change train if there is no match
            delay = CHANGE_TIME;

            char* new_best_train = find_best_train(graph, ptr->allowed_trains, prev, end_pos ,pos);     //create a string with the name of the train that can go the furthest from the current position
            strcpy(chosen_train, new_best_train);
            free(new_best_train);

            printf("\nTrain changed to \"%s\"", chosen_train);
        }
        return delay + extra_delay(graph, prev[pos], end_pos, prev, chosen_train);
    }
}
