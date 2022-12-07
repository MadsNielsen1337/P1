#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "routes.h"
#include "trains.h"
#include "graph.h"

//Take a dot seperated string and target string and returns 1 if the target string matches a part of the dot seperated string
int segmented_string_compare(const char* str, const char* target) {
    char to_search[DATA_SIZE];                                 // string to store input string
    strcpy(to_search, str);                        // copying input string into to_search
    int result = 0;                                           //The outcome of the function


    for (int i = strlen(to_search) - 1; i >= 0; i--) {     //for loop that search through the string

        if (to_search[i] == '.' || i == 0) {                   //check if there is a dot in the string or if it is at the end of the string

            if (i != 0) {                                      //If i is not at the end of the string we replace dot with an end character
                to_search[i] = '\0';
                if (strcmp(&to_search[i + 1], target) == 0) {      //Compares input string to the target and if 'true' returns 1
                    result = 1;
                }
            } else {
                if (strcmp(&to_search[i], target) == 0){
                    result = 1;
                }
            }

        }
    }
    return result;
}

//Takes train array and a route and compare all trains with route to put train into edge
char* compatible_trains(const train* t, const route r, const int train_count){
    char* compatible_trains = malloc(DATA_SIZE * sizeof(char));
    compatible_trains[0] = '\0';

    for(int i = 0; i < train_count; i++){                                           //For loop that goes through train data and compares control system and fuel/power
        if(segmented_string_compare(t[i].controls, r.controls) == 1){     //Compares controls for train and route
            if(segmented_string_compare(t[i].fuels, "diesel") == 1){      //Compares fuels to diesel
                strcat(compatible_trains, t[i].name);                    //
                strcat(compatible_trains, ".");
            }
            else if(segmented_string_compare(t[i].fuels, r.power)){       //Compares any other fuels/power for train and route
                strcat(compatible_trains, t[i].name);                    //
                strcat(compatible_trains, ".");
            }
        }
    }
    int last_char = strlen(compatible_trains) - 1;
    if(compatible_trains[last_char] == '.'){
        compatible_trains[last_char] = '\0';
    }
    return compatible_trains;
}