#ifndef P1_TRAINS_H
#define P1_TRAINS_H

#define DATA_SIZE 40
#define XL_DATA_SIZE 140

typedef struct train{
    char name[DATA_SIZE];
    char gauge[DATA_SIZE];
    char controls[DATA_SIZE];
    char fuels[DATA_SIZE];
    double acceleration;
    double max_speed;
    int passenger_space;
} train;

int segmented_string_compare(const char* str, const char* target);
char* compatible_trains(const train* t, route r, int train_count);

#endif //P1_TRAINS_H
