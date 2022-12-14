#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "routes.h"
#include "trains.h"
#include "time.h"
//#include "graph.h"

//Take a dot seperated string and target string and returns 1 if the target string matches a part of the dot seperated string
int segmented_string_compare(const char* str, const char* target) {
    char to_search[XL_DATA_SIZE];                                 // string to store input string
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
    char* compatible_trains = malloc(XL_DATA_SIZE * sizeof(char));
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

char* select_random_train(const char* str) {
    char to_search[XL_DATA_SIZE];// string to store input string
    char* out = malloc(sizeof(char) * DATA_SIZE);
    int dot_count = 0;
    strcpy(to_search, str);                        // copying input string into to_search
    printf("Input string %s\n", to_search);

    for (int i = strlen(to_search) - 1; i >= 0; i--) {
        if (to_search[i] == '.') {
            dot_count++;
        }
    }
    int index = dot_count + 1;
    int index_arr[strlen(to_search) - dot_count];
    int j = 0;

    for (int i = strlen(to_search) - 1; i >= 0; i--) {     //for loop that search through the string
        if(strlen(to_search) < 1) {
            printf("\nError: String size %zu is less than allowed string size 2\n", strlen(to_search));
            return NULL;
        }
        if(to_search[i] == '.' || i == 0) {
            index_arr[j] = i + 1;
            j++;
            index--;
            if(index == 0) {
                srand(time(NULL));
                int rng = (rand() % (j + 1));
                //printf("\nString size: %zu\n", strlen(to_search));
                //printf("j = %d\n", j);
                //printf("\nIndex nr. %d\n", index_arr[rng]);
                strcpy(out, &to_search[index_arr[rng]]);
                return out;
            }
            else if(index < 0) {
                printf("\nError: Index out of bounds by %d\n", index);
                return NULL;
            }
            to_search[i] = '\0';
        }

    }
    return NULL;
}
