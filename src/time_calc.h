#ifndef P1_TIME_CALC_H
#define P1_TIME_CALC_H
#include "graph.h"

double time(route r, train t);
double added_delay(route r, train t);
int weight_calc(route r, train t);

int segmented_string_length(char* string);
char* find_best_train(const struct Graph* graph, char* chosen_trains,const int* prev, int end_pos, int pos);
int extra_delay(const struct Graph* graph, int pos, int end_pos, const int* prev, char* chosen_train);

#endif //P1_TIME_CALC_H
