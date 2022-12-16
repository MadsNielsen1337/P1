#ifndef P1_TIME_CALC_H
#define P1_TIME_CALC_H
#include "graph.h"

double time(route r, train t);
double added_delay(route r, train t);
int weight_calc(route r, train t);

int segmented_string_length(char* string);
char* find_best_train(const struct Graph* graph, char* chosen_trains,const int* prev, int end_pos, int pos);
int extra_delay(const struct Graph* graph, int pos, int end_pos, const int* prev, char* chosen_train);


float average_weight_difference(const float* a, const float* b, int array_length);
float percentage_weight_difference(const float* a, const float* b, int array_length);
int delay_optimised(const struct Graph* graph, char* chosen_trains, const int* prev, int end_pos, int pos);

float average_simple(const float* average, int length_of_array);
float highest_num(const float* dist, int length_of_array);
float lowest_num(const float* dist, int length_of_array);
float median_finder(const float* dist, int length_of_array);


#endif //P1_TIME_CALC_H
