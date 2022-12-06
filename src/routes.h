#ifndef P1_ROUTES_H
#define P1_ROUTES_H

#define DATA_SIZE 40

// Struct that holds data for routes - aka. the template
typedef struct route{
    int distance;
    int track_speed;
    char power[DATA_SIZE];
    char gauge[DATA_SIZE];
    char controls[DATA_SIZE];
    char station_start[DATA_SIZE];
    char station_end[DATA_SIZE];
} route;

// Struct that holds the linked list - aka. the name of route 'i' and a pointer to the next list.
typedef struct station_list_node{
    char name[DATA_SIZE];
    struct station_list_node* next;
}station_list_node;

// Function that begins a linked list of station names
station_list_node* create_station_list(const char* first_name);

// Recursive function that tries to add a new name to the station list if it is not already there
void add_node(station_list_node* node, const char* new_name);

// Function to print all names stored in the station list, from first to last
void print_station_list(station_list_node* node);

// Function to create and fill a linked list of station names with data from the route array. Returns a pointer to the start of the created list
station_list_node* build_station_list(route* r, const int* route_count);

// Function to find the length of an existing station list
int list_length(station_list_node* node);

// Recursive part of search_station_list. the depth variable should always be set to 1 at the beginning of a search
int search_list_recursive(station_list_node* node, const char* searched_name, int depth);

// Function that returns an integer based on how far into the list a name is. Returns -1 if searched name is not in the list
int search_station_list(station_list_node* list, const char* searched_name);

// Returns the name at a given index of a station list starting from the location pointed to by the node variable. Does not support negative index values
char* index_station_list(station_list_node* node, int index);   // changed 'const int' to 'int'. The function definition is still the same though, as it only affects definitions, not declarations.

// Function to test the linked station list's basic functions
void list_test();

#endif //P1_ROUTES_H
