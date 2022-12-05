#ifndef P1_TRAINS_H
#define P1_TRAINS_H

#define DATA_SIZE 40

typedef struct train{
    char name[DATA_SIZE];
    char gauge[DATA_SIZE];
    char controls[DATA_SIZE];
    char fuels[DATA_SIZE];
    double acceleration;
    double max_speed;
    int passenger_space;
} train;

#endif //P1_TRAINS_H
