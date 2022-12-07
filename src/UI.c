#include <stdio.h>
#include <stdlib.h>
#include "routes.h"

#define UI_SIZE 61
#define SPACING 2

// -----------------------------------------------------------------------------------
// Local prototypes. Globals are declared in UI.h

void add_spacing(int spacing);
void print_long_line(int ui_size);
void print_long_line_pipe(int ui_size);
void print_long_line_equals(int ui_size);

// Garbage collector. Keeps reading the input until a new line (or the end of the file) is reached
void clear_input(void);

// Draws a UI in the terminal
void drawMenu(void);

// Processes the user input
int menu_choice(route* routes, station_list_node* list_of_stations);

void list_all_stations(station_list_node* list_of_stations);

// -----------------------------------------------------------------------------------

// Garbage collector.
void clear_input(void){
    int garbageCollector;
    while ((garbageCollector = getchar()) != '\n' && garbageCollector != EOF)   //keep reading the input until a new line (or the end of the file) is reached
    {}
}

void add_spacing(int spacing)
{
    char drawMenu = '|';
    for (int i = 0; i < spacing; ++i) {
        printf("\n%c%60c", drawMenu, drawMenu);     // Add space between text
    }
}

void print_long_line(int ui_size)
{
    char drawMenu = '_';
    printf("\n");
    for (int i = 0; i < ui_size; ++i) {
        printf("%c", drawMenu);     // Print the UI header
    }
}

void print_long_line_pipe(int ui_size)
{
    char drawMenu = '_';
    char drawPipe = '|';
    printf("\n");
    printf("%c", drawPipe);
    for (int i = 0; i < ui_size - 2; ++i) {
        printf("%c", drawMenu);     // Print the UI header
    }
    printf("%c", drawPipe);
}

void print_long_line_equals(int ui_size)
{
    char drawEquals = '=';
    printf("\n");
    for (int i = 0; i < ui_size; ++i) {
        printf("%c", drawEquals);     // Print the UI header
    }
}



// Draws a UI in the terminal
void drawMenu(void)
{
    char drawMenu = '|';

    print_long_line_equals(UI_SIZE);

    add_spacing(SPACING - 1);

    printf("\n%c UI of European railway simulation%26c", drawMenu, drawMenu);
    printf("\n%c Made by Emil, Eva, Frederik, Louise, Mads & Mohamad%8c", drawMenu, drawMenu);

    add_spacing(SPACING - 1);

    print_long_line_pipe(UI_SIZE);

    add_spacing(SPACING - 1);

    printf("\n%c [r] Run simulation (placeholder)%27c", drawMenu, drawMenu);
    printf("\n%c [s] See all available stations%29c", drawMenu, drawMenu);
    printf("\n%c [q] Exit%51c", drawMenu, drawMenu);

    add_spacing(SPACING - 1);

    print_long_line_equals(UI_SIZE);

    printf("\n");



/* // Tests to see if the format is right
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
*/

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
        printf("\nChoose menu option:");
        scanf("%c", &choice);       // Read input from user
        clear_input();

        // Since the while loop can't break for some reason these if-else statements do the job.
        if (choice == 'q')                  // Quit
            exit(EXIT_SUCCESS);
        else if (choice == 'a') {           // UNDEFINED
            // call a function
            printf("a = fisk\n");
        }
        else if (choice == 's') {           // Show all stations
            list_all_stations(list_of_stations);
        }
        else if (choice == 'b') {           // Return to main menu
            drawMenu();
        }
    }
}

void list_all_stations(station_list_node* list_of_stations)
{
    print_long_line_equals(UI_SIZE);
    printf("\n");
    printf("\nHere are all available stations:\n");

    // Dynamically prints ALL available stations and their index.
    for (int i = 0; i < (list_length(list_of_stations)); ++i) {
        //printf("\n%c [%d] %s",drawMenu, i, routes[i].station_start);
        printf("\n [%d] %s", i,index_station_list(list_of_stations, i));
    }

    printf("\n");
    printf("\n [b] Return tp main menu");
    printf("\n");
    print_long_line(UI_SIZE);
}

// Encapsulates all UI functions into one for ease of use
void GenerateUI(route* routes, station_list_node* list_of_stations)
{
    drawMenu();
    menu_choice(routes, list_of_stations);
}