#include <math.h>
#include "routes.h"
#include "trains.h"
#include "time_calc.h"
#include "graph.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define METER_CONVERSION 1000               // Multiply this with km to get m
#define METER_PER_SECOND_CONVERSION 3.6     // Divide this with km/h to get m/s
#define CHANGE_DELAY 900                     // Time to change trains in seconds. Obviously an estimation
#define BASE_DELAY 300                      // Wait time at each station to alloy passengers to get on and off in seconds. Obviously an estimation


// -----------------------------------------------------------------------------------
// Local prototypes. Globals are declared in UI.h



// -----------------------------------------------------------------------------------

// Calculate the time it takes to go from station_start to station_end. Takes accel & decel into account
double time(route r, train t)
{
    // Since data about decel is hard to find, it is expected that accel = decel.

    // Accel
    // v = a * t + v_0

    // t = (v-v_0)/a

    // Include the distance
    // t = sqrt(2 * a * s + v_0^2) - v_0 / a

    if(r.track_speed < t.max_speed)
        t.max_speed = r.track_speed;

    double time_max_speed,
           s,
           time,
           average_speed_half,
           final_time;

    t.max_speed /= METER_PER_SECOND_CONVERSION;
    r.distance *= METER_CONVERSION;

    // The time it takes to accelerate from v = 0 to t.max_speed
    time_max_speed = t.max_speed / t.acceleration; // Unit: s

    // s = 1/2 * a * t^2 + v_0 * t
    // The distance travelled with t.accel and time_max_speed starting from v = 0. Essentially the accel distance.
    s = 1.0/2.0 * t.acceleration * pow(time_max_speed, 2); // Unit: m

    // t = s / v_0  ->  t * v_0 = s  ->  v_0 = s / t
    // The average speed of half of t.accel distance. This corresponds to t.max_speed in the accel/decel phase
    average_speed_half = ((s / 2) / time_max_speed); // Unit: m/s

    // The time it takes to travel the accel/decel distance with the average accel/decel speed.
    final_time = (2 * s) / average_speed_half; // Unit: s

    // The time it takes to travel r.distance. With greater precision since accel/decel calculations have been refined
    time = ((r.distance - (2 * s)) / t.max_speed) + final_time + BASE_DELAY; // Unit: s

    //printf("\naccel speed phase %.0lf m/s reaching t.max_speed %.0lf m/s (%.0lf km/h) in accel_dist %0.lf m taking %.0lf s (%.0lf m)\n Total dist: %d m\n", average_speed_half, t.max_speed, t.max_speed*METER_PER_SECOND_CONVERSION, s, final_time_large, final_time_large/60, r.distance);

    return time;
}

// Calculates the weight for the edge going from station_start to station_end
int weight_calc(route r, train t)
{
    int weight;    // Delay time in minutes

    weight = time(r, t);

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
    char selected_train[XL_DATA_SIZE];  //initialise an array to hold the train we wish to check for in allowed_trains

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

//this function finds the extra delay (as defined by CHANGE_DELAY) added from travelling from pos to end_pos via the prev-array generated by the dijkstra function. Requires a train to be chosen before it is run. Note that end_pos is the position where the dijkstra-function begins its pathfinding.
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
            delay = CHANGE_DELAY;

            char* new_best_train = find_best_train(graph, ptr->allowed_trains, prev, end_pos ,pos);     //create a string with the name of the train that can go the furthest from the current position
            strcpy(chosen_train, new_best_train);
            free(new_best_train);

            //printf("\nTrain changed to \"%s\"", chosen_train);
        }
        return delay + extra_delay(graph, prev[pos], end_pos, prev, chosen_train);
    }
}

//function to calculate the average difference between all entries in two arrays of the same size
float average_weight_difference(const float* a, const float* b, const int array_length){
    float differences[array_length];
    for (int i = 0; i < array_length; ++i) {          //find and add differences to an array
        differences[i] = a[i] - b[i];
        //printf("\n[%d] Difference %f = a[%f] - b[%f]\n",i, differences[i], a[i], b[i]);
    }
    float sum = 0;
    for (int i = 0; i < array_length; ++i) {          //find sum of non-zero values in that array
        sum += differences[i];
    }
    //printf("\nSum = %f\n", sum);
    return sum/(float)array_length;  //return sum divided by number of entries i.e. the average
}

//function to calculate the percentage difference between all entries in two arrays of the same size
float percentage_weight_difference(const float* a, const float* b, const int array_length){
    float differences[array_length];
    for (int i = 0; i < array_length; ++i) {          //find and add differences to an array
        differences[i] = a[i] - b[i];
    }

    float percentage[array_length];
    for (int i = 0; i < array_length; ++i) {          //find the percentage that difference makes up of original value and add them to an array
        if(differences[i] != 0 && b[i] != 0) {
            percentage[i] = (differences[i] / b[i]) * 100;
        } else {
            percentage[i] = 0;
        }
    }
    float sum = 0;
    for (int i = 0; i < array_length; ++i) {          //find sum of non-zero values in that array
        sum += percentage[i];
    }
    return sum/(float)array_length;  //return sum divided by number of entries i.e. the average
}

//end_pos is the position where the dijkstra-function that generated the prev-array began its pathfinding, pos is a given node in the graph and chosen_trains should begin as the trains allowed on the initial route
int delay_optimised(const struct Graph* graph, char* chosen_trains, const int* prev, const int end_pos, const int pos){
    if(pos != end_pos) {
        struct Node *ptr = graph->head[pos];
        while (ptr != NULL && ptr->dest != prev[pos]) {     //find the correct route to check for matching trains on
            ptr = ptr->next;
        }

        char new_trains[XL_DATA_SIZE];      //initialise an array to hold matching trains and let the first spot be the end, so strcat can be used to build it
        new_trains[0] = '\0';
        char to_search[XL_DATA_SIZE];       //initialise an array to hold a copy of chosen trains, so we can do work on it
        strcpy(to_search, chosen_trains);
        char selected_train[XL_DATA_SIZE];  //initialise an array to hold the train we wish to check for in allowed_trains

        for (int i = strlen(chosen_trains); i >= 0; --i) {
            if (to_search[i] == '.' || i == 0) {
                if(i != 0){
                    to_search[i] = '\0';
                    strcpy(selected_train, &to_search[i + 1]);                        //copy the rightmost entry in to_search into selected_train and prepare the next one
                } else {
                    strcpy(selected_train, &to_search[i]);                            //ditto but without preparing the next one, as this is the last entry in the string
                }
                if (segmented_string_compare(ptr->allowed_trains, selected_train)) {    //if the selected_train is in allowed_trains for this route, add it to the new_trains-string
                    strcat(new_trains, selected_train);
                    strcat(new_trains, ".");
                    //printf("\tAdding: %s", selected_train);
                }
            }
        }

        int last_char = strlen(new_trains) - 1;               //format new segmented string correctly once it has been created
        if (new_trains[last_char] == '.') {
            new_trains[last_char] = '\0';
        }

        if(segmented_string_length(new_trains) > 0){                                                              //check if there are any trains still allowed on the current route
            //printf("\nProceeding with: %s", new_trains);
            return delay_optimised(graph, new_trains, prev, end_pos, prev[pos]);                        //run the function on the next part of the path with the trains we have found to be compatible
        } else {
            //printf("\nCHANGING to: %s from %s", ptr->allowed_trains, chosen_trains);
            return CHANGE_DELAY + delay_optimised(graph, ptr->allowed_trains, prev, end_pos, prev[pos]); //add a delay and run the function on the next part of the path with the trains we know can run on it
        }
    } else {
        return 0;
    }
}

//Function that take an array and the length of the array and finds the average of the array
float average_simple(const float* average, const int length_of_array){
    float sum = 0, result = 0;                                        //Two variables containing the sum of the array and the result which is the average of the array
    for(int i = 0; i < length_of_array; i++){                         //For loop that sums everything inside array
        sum += average[i];
        //printf("\n%lf", average[i]);
    }
    result = sum/(float)length_of_array;                               //Calculates the average of the array using sum and length of array
    return result;                                                     //returns the average
}

float highest_num(const float* dist, const int length_of_array){
    float current_num = 0;
    for(int i = 0; i < length_of_array; i++){
        if(dist[i] > current_num){
            current_num = dist[i];
        }
    }
    return current_num;
}

float lowest_num(const float* dist, const int length_of_array) {
    float current_num = 0;
    for (int i = 0; i < length_of_array; i++) {
        if (dist[i] < current_num) {
            current_num = dist[i];
        }
    }
    return current_num;
}

float median_finder(const float* dist, const int length_of_array){
    float result = 0;
    if(length_of_array % 2 == 0){
        return dist[length_of_array/2];
    } else {
        result = dist[(int)floor(length_of_array)]+dist[(int)ceil(length_of_array)]/2;
        return result;
    }
}