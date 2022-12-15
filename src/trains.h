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
//char** compatible_trains(route r, train*, int length_of_train_list);

//void convert_controls(train* t, route r, int train_count);
int segmented_string_compare(const char* str, const char* target);
char* compatible_trains(const train* t, route r, int train_count);
char* select_random_train(const char* str);
char* select_all_trains(const char* str, int array_index);
int select_all_trains_helper(const char* str);
#endif //P1_TRAINS_H
