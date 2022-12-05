#include <stdio.h>
#include <stdlib.h>
#include "routes.h"

#define UI_SIZE 61
#define UI_SPACING 2

// Garbage collector.
void clear_input(void){
    int garbageCollector;
    while ((garbageCollector = getchar()) != '\n' && garbageCollector != EOF)   //keep reading the input until a new line (or the end of the file) is reached
    {}
}

// Draws a UI in the terminal
void drawMenu(route* routes, station_list_node* list_of_stations) // This function is a nightmare. Needs some cleanup
{
    char drawMenu = '-';

    printf("\n");
    for (int i = 0; i < UI_SIZE; ++i) {
        printf("%c", drawMenu);     // Print the UI header
    }
    for (int i = 0; i < UI_SPACING - 1; ++i) {
        printf("\n%c%60c", drawMenu, drawMenu);     // Add space between text
    }
    printf("\n%c UI of European railway simulation%26c", drawMenu, drawMenu);
    printf("\n%c Made by Emil, Eva, Frederik, Louise, Mads & Mohamad%8c", drawMenu, drawMenu);
    for (int i = 0; i < UI_SPACING; ++i) {
        printf("\n%c%60c", drawMenu, drawMenu);     // More space
    }

    for (int i = 0; i < (list_length(list_of_stations)); ++i) {   // Dynamically prints ALL available stations and their index.
        //printf("\n%c [%d] %s",drawMenu, i, routes[i].station_start);
        printf("\nS[%d] '%s'", i,index_station_list(list_of_stations, i));
    }
    printf("\n");
    for (int i = 0; i < list_length(list_of_stations); ++i) {
        printf("\nP[%d] '%s'", i, routes[i].power);
    }
    printf("\n");
    for (int i = 0; i < list_length(list_of_stations); ++i) {
        printf("\nG[%d] '%s'", i, routes[i].gauge);
    }
    printf("\n");
    for (int i = 0; i < list_length(list_of_stations); ++i) {
        printf("\nC[%d] '%s'", i, routes[i].controls);
    }

    printf("\nlist_length: %d\n", list_length(list_of_stations));
    print_station_list(list_of_stations);

    printf("\n");
    for (int i = 0; i < UI_SIZE; ++i) {
        printf("%c", drawMenu);     // Print long line separation
    }
    for (int i = 0; i < UI_SPACING; ++i) {
        printf("\n%c%60c", drawMenu, drawMenu);     // More space
    }
    printf("\n%c [q] Exit%51c", drawMenu, drawMenu);
    for (int i = 0; i < UI_SPACING; ++i) {
        printf("\n%c%60c", drawMenu, drawMenu);     // Even more space
    }
    printf("\n");
    for (int i = 0; i < UI_SIZE; ++i) {
        printf("%c", drawMenu);     // Print UI footer
    }
    printf("\n");
}

// Processes the user input
int menu_choice(route* routes, station_list_node* list_of_stations) {
    /*
        fscanf(stdin, "%[^A-Za-z0-9]", &choice);    // Read until a letter or number is found.
        fscanf(choice, "%[^q]", choice);            // Check if 'q' was input
        if(choice == 'q') {
            exit(EXIT_SUCCESS);
             strtol(&choice, NULL, 10)
        }
                for(int i = 0; i < 10; ++i) {
            if(choice == i) {
                // load graph
                // this is our starting station
            }
     */
    char choice = ' ';
    while (choice != 'q' || choice != 'a' || choice != 'b') {
        printf("\nChoose menu option: ");
        scanf("%c", &choice);       // Read input from user
        clear_input();

        if (choice == 'q')       // Since the while loop can't break for some reason these if-else statements do the job.
            exit(EXIT_SUCCESS);
        else if (choice == 'a') {
            // call a function
            printf("a = fisk\n");
        } else if (choice == 'b') {
            // call a function
            printf("b = fisk2\n");
        }
    }
}

// Encapsulates all UI functions into one for ease of use
void GenerateUI(route* routes, station_list_node* list_of_stations)
{
    drawMenu(routes, list_of_stations);
    menu_choice(routes, list_of_stations);
}