#ifndef P1_UI_H
#define P1_UI_H
#include <stdio.h>
#include <stdlib.h>
#include "routes.h"

#define UI_SIZE 61
#define UI_SPACING 2
char drawMenu = '-';


void clear_input(void){
    int garbageCollector;
    while ((garbageCollector = getchar()) != '\n' && garbageCollector != EOF)   //keep reading the input until a new line (or the end of the file) is reached
    {}
}

int drawMenu1(void) {
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
}
int drawMenu2(route* routes, station_list_node* list_of_stations)
{
    for (int i = 0; i < list_length(list_of_stations); ++i) {   // Dynamically prints ALL available stations and their index.
        printf("\n%c [%d] %s%31c", drawMenu, (list_of_stations, routes[i].station_start), routes[i].station_start, drawMenu);
    }
}

int drawMenu3(void)
{
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

        if (choice ==
            'q')       // Since the while loop can't break for some reason these if-else statements do the job.
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
void GenerateUI(route* routes, station_list_node* list_of_stations)
{
    drawMenu1();
    drawMenu2(routes, list_of_stations);
    drawMenu3();
    menu_choice(routes, list_of_stations);
}



#endif //P1_UI_H
