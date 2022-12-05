#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "routes.h"
#include "trains.h"

// Function that begins a linked list of station names
station_list_node* create_station_list(const char* first_name)
{
    station_list_node* list = malloc(sizeof(station_list_node));   //create the first node using malloc
    list->next = NULL;                                                  //set the pointer to the next node to NULL since the first node is also the last right now
    strcpy(list->name ,first_name);                         //add the first station name to the list
    return list;
}

// Recursive function that tries to add a new name to the station list if it is not already there
void add_node(station_list_node* node, const char* new_name)
{
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

// Function to print all names stored in the station list, from first to last
void print_station_list(station_list_node* node)
{
    printf("%s\n", node->name);
    if(node->next != NULL){
        print_station_list(node->next);
    }
}

// Function to create and fill a linked list of station names with data from the route array. Returns a pointer to the start of the created list
station_list_node* build_station_list(route* r, int* route_count)
{
    station_list_node* list = create_station_list(r[0].station_start);     //use the first data point to begin the list

    for (int i = 0; i < *route_count; ++i){
        add_node(list, r[i].station_start); //run add_node with every station_start and station_end value
        add_node(list, r[i].station_end);
    }
    return list;
}

// Function to find the length of an existing station list
int list_length(station_list_node* node)
{
    if(node->next == NULL){
        return 1;
    } else {
        return 1 + list_length(node->next);
    }
}

// Recursive part of search_station_list. the depth variable should always be set to 1 at the beginning of a search
int search_list_recursive(station_list_node* node, const char* searched_name, int depth)
{
    if(strcmp(node->name, searched_name) == 0){
        return 0;
    } else {
        if(node->next == NULL){
            return -(depth);
        } else {
            return 1 + search_list_recursive(node->next, searched_name, depth + 1);
        }
    }
}

// Function that returns an integer based on how far into the list a name is. Returns -1 if searched name is not in the list
int search_station_list(station_list_node* list, const char* searched_name)
{
    return search_list_recursive(list, searched_name, 1);   //runs the recursive search function with depth 1 to make it possible to return -1
}

// Returns the name at a given index of a station list starting from the location pointed to by the node variable. Does not support negative index values
char* index_station_list(station_list_node* node, const int index)
{
    if(index == 0){
        return node->name;
    } else {
        if(node->next == NULL){
            printf("\nWARNING: Station list size exceeded by %d", index);
            return ("INDEX OUT OF BOUNDS");
        } else {
            return (index_station_list(node->next, index - 1));
        }
    }
}

// Function to test the linked station list's basic functions
void list_test()
{
    station_list_node* list = create_station_list("Test:");
    char name[DATA_SIZE];
    for (int i = 0; i < 5; ++i) {
        scanf("%s", name);
        add_node(list, name);
    }
    print_station_list(list);
    printf("\nList has a length of %d\n", list_length(list));
    printf("\nWord \"hello\" is at index %d\n" ,search_station_list(list, "hello"));
    printf("\nWord at the 3rd index is \"%s\"", index_station_list(list, 3));
}