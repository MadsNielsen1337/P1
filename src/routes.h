#ifndef P1_ROUTES_H
#define P1_ROUTES_H
#define DATA_SIZE 40
#define ROUTE_COUNT 3
#include <string.h>
// Data taken from "https://www.raileurope.com/en"
// For more info see "https://www.raileurope.com/blog/travel-green-calculating-your-carbon-savings"

// When multiple routes for the same journey are available, the route with shortest travel time is selected.

// All stations are of format "SOURCE_DESTINATION".

typedef struct route{
    int distance;
    int track_speed;
    char power[DATA_SIZE];
    char gauge[DATA_SIZE];
    char controls[DATA_SIZE];
    char station_start[DATA_SIZE];
    char station_end[DATA_SIZE];
} route;

void scan_routes(FILE* p_file, route* r){   //reads from a file where each line has the following format: Distance,Track_speed,Power,Gauge,Controls,Station_Start,Station_End
    char temp[DATA_SIZE];
    for (int i = 0; i < ROUTE_COUNT; ++i) {

        fscanf(p_file, "%[^,]", temp);
        r[i].distance = strtol(temp, NULL, 10);  //using strtol here to convert the string we've read into a long integer
        fscanf(p_file, "%*[,]");
        fscanf(p_file, "%[^,]", temp);
        r[i].track_speed = strtol(temp, NULL, 10);
        fscanf(p_file, "%*[,]");
        fscanf(p_file, "%[^,]", r[i].power);
        fscanf(p_file, "%*[,]");
        fscanf(p_file, "%[^,]", r[i].gauge);        //read until a comma is found
        fscanf(p_file, "%*[,]");                    //dump the comma
        fscanf(p_file, "%[^,]", r[i].controls);     //read until the next comma
        fscanf(p_file, "%*[,]");                    //and so on
        fscanf(p_file, "%[^,]", r[i].station_start);
        fscanf(p_file, "%*[,]");
        fscanf(p_file, "%[^\n]", r[i].station_end); //station_end is the last piece of data. a newline character comes right after it
        fscanf(p_file, "%*[^a-zA-Z0-9]");           //skip characters until a letter or number is reached, which would be at the next line
    }
    //printf to test if the format is right. delete later
    printf("\n%d %d %s %s %s %s %s\n",r[0].distance,r[0].track_speed,r[0].power,r[0].gauge,r[0].controls,r[0].station_start,r[0].station_end);
}

typedef struct station_list_node{
    char name[DATA_SIZE];
    struct station_list_node* next;
}station_list_node;

//function that begins a linked list of station names
station_list_node* create_station_list(const char* first_name){
    station_list_node* list = malloc(sizeof(station_list_node));   //create the first node using malloc
    list->next = NULL;                                                  //set the pointer to the next node to NULL since the first node is also the last right now
    strcpy(list->name ,first_name);                         //add the first station name to the list
    return list;
}

//recursive function that tries to add a new name to the station list if it is not already there
void add_node(station_list_node* node, const char* new_name){
    if(strcmp(node->name, new_name) != 0){                                       //if the name does not match,
        if(node->next == NULL){                                                  //we check if we are at the end of the list
            station_list_node* new_node = malloc(sizeof(station_list_node));//and add a new node if we are
            new_node->next = NULL;                                               //mostly the same as creating the list, except we set the pointer from the current node to point to the new node instead of NULL
            strcpy(new_node->name ,new_name);
            node->next = new_node;
        } else {                                                                 //if we are not at the end,
            add_node(node->next, new_name);                                //we try again at the next list node
        }
    }
}

//function to print all names stored in the station list, from first to last
void print_station_list(station_list_node* node){
    printf("%s\n", node->name);
    if(node->next != NULL){
        print_station_list(node->next);
    }
}

void list_test(){   //function to test the linked station list
    station_list_node* list = create_station_list("Test:");
    char name[DATA_SIZE];
    for (int i = 0; i < 5; ++i) {
        scanf("%s", name);
        add_node(list, name);
    }
    print_station_list(list);
}


#endif //P1_ROUTES_H
