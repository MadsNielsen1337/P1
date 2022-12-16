#include <stdio.h>
#include <stdlib.h>
#include "routes.h"
#include "graph.h"
#include "time_calc.h"

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
void menu_choice(int node_count, station_list_node* list_of_stations, struct Graph* graph, float distance[], int previous_node[], float new_dist[], float average[], float percent[]);

void list_all_stations(station_list_node* list_of_stations);

// Run dijkstra and its associated functions
void path_finder(int node_count, station_list_node* list_of_stations, struct Graph* graph, float distance[], int previous_node[], float new_dist[], float average[], float percent[]);

// -----------------------------------------------------------------------------------

// Garbage collector.
void clear_input(void){
    int garbageCollector;
    while ((garbageCollector = getchar()) != '\n' && garbageCollector != EOF)   // Keeps reading the input until a new line (or the end of the file) is reached
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
    printf("\n%c [g] Print graph%44c", drawMenu, drawMenu);
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
void menu_choice(int node_count, station_list_node* list_of_stations, struct Graph* graph, float distance[], int previous_node[], float new_dist[], float average[], float percent[])
{
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
    char draw_Menu = '|';
    char menuSpacing = ' ';
    char choice = ' ';
    while (choice != 'q') {
        printf("\nChoose menu option:");
        scanf("%c", &choice);                // Read input from user
        clear_input();

        switch (choice) {
            case 'q':   // Quit
                exit(EXIT_SUCCESS);
            case 'r':                               // Dijkstra
            {
                path_finder(node_count, list_of_stations, graph, distance, previous_node, new_dist, average, percent);
                drawMenu();
                break;
            }
            case 's':                           // Show all stations
                list_all_stations(list_of_stations);
                printf("\n\n");
                drawMenu();
                break;
            case 'g':                           // Print graph
                printf("\n");
                print_long_line_equals(UI_SIZE);
                printf("\n%c%19cPrinted adjacency list%19c", draw_Menu, menuSpacing, draw_Menu);
                print_long_line_equals(UI_SIZE);
                printf("\n");
                printGraph(graph);
                printf("\n");
                print_long_line_equals(UI_SIZE);
                printf("\n%c%26cList end%26c", draw_Menu, menuSpacing, draw_Menu);
                print_long_line_equals(UI_SIZE);
                printf("\n\n");
                drawMenu();
                break;
            default:
                printf("\nDEFAULT\n");
                printf("Choice = '%c'\n", choice);
                drawMenu();
        }
    }
}

void list_all_stations(station_list_node* list_of_stations)
{
    char draw_Menu = '|';
    char menuSpacing = ' ';

    printf("\n");
    print_long_line_equals(UI_SIZE);
    printf("\n%c%14cHere are all available stations%15c", draw_Menu, menuSpacing, draw_Menu);
    print_long_line_equals(UI_SIZE);
    printf("\n");

    // Dynamically prints ALL available stations and their index.
    for (int i = 0; i <= 9; ++i) {
        printf("\n [%d] %s", i,index_station_list(list_of_stations, i));
    }
    for (int i = 10; i < (list_length(list_of_stations)); ++i) {
        //printf("\n%c [%d] %s",drawMenu, i, routes[i].station_start);
        printf("\n [%d] %s", i,index_station_list(list_of_stations, i));
    }
    printf("\n");
    print_long_line_equals(UI_SIZE);
    printf("\n%c%23cEnd of stations%22c", draw_Menu, menuSpacing, draw_Menu);
    print_long_line_equals(UI_SIZE);
    printf("\n");
}

void path_finder(int node_count, station_list_node* list_of_stations, struct Graph* graph, float distance[], int previous_node[], float new_dist[], float average[], float percent[])
{


    for (int i = 0; i < node_count; ++i) {
        dijkstra(graph, distance, previous_node, i, list_length(list_of_stations));

        printf("\nDIJKSTRA %d", i);

        /*for (int j = 0; j < node_count; ++j) {
            //printf("\n%f + %f", dist[j],(float)delay_optimised(graph, start_train , prev, i, j ));
        }*/

        for (int j = 0; j < node_count; ++j) {
            struct Node *ptr = graph->head[j];
            while (ptr != NULL && ptr->dest != previous_node[j]) {     //find the correct route to check for matching trains on
                ptr = ptr->next;
            }
            if(i != j) {
                printf("\n[%d] Delay optimised: %lf\n", j, (float)delay_optimised(graph, ptr->allowed_trains, previous_node, i, j));
                printf("[%d] Dist: %f\n", j, distance[j]);
                printf("[%d] New dist: %f\n", j, new_dist[j]);


                new_dist[j] = distance[j] + (float) delay_optimised(graph, ptr->allowed_trains, previous_node, i, j);
                new_dist[i] = 0;
                printf("\n[2 | %d] Delay optimised: %lf\n", j, (float)delay_optimised(graph, ptr->allowed_trains, previous_node, i, j));
                printf("[2 | %d] Dist: %f\n", j, distance[j]);
                printf("[2 | %d] New dist: %f\n", j, new_dist[j]);
                /*
                if(900 < (float) delay_optimised(graph, ptr->allowed_trains, prev, i, j)){
                    printf("Starting with: %s", ptr->allowed_trains);
                    printf("\nOptimised: %lf\n", (float) delay_optimised(graph, ptr->allowed_trains, prev, i, j));
                    printf("Not Optimised: %lf\n", (float) extra_delay(graph, j, i, prev, start_train));
                }
                 */
            }
        }
        average[i] = average_weight_difference(new_dist, distance, node_count);
        percent[i] =  percentage_weight_difference(new_dist, distance, node_count);
        printf("\n[%d] Average delay is %f", i, average_weight_difference(new_dist, distance, node_count));
        printf("\n[%d] Average extra time in percent %f", i,  percent[i]);
        printf("\n\n");
    }
    printf("\nAverage of averages: %lf\n", average_simple(average, node_count));
    printf("Average of averages in percent: %lf\n", average_simple(percent, node_count));
}

// Encapsulates all UI functions into one for ease of use
void GenerateUI(int node_count, station_list_node* list_of_stations, struct Graph* graph, float distance[], int previous_node[], float new_dist[], float average[], float percent[])
{
    drawMenu();
    menu_choice(node_count, list_of_stations, graph, distance, previous_node, new_dist, average, percent);
}