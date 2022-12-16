#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "routes.h"
#include "graph.h"
#include "time_calc.h"

#define UI_SIZE 61
#define LARGE_UI_SIZE 86
#define SPACING 2

// -----------------------------------------------------------------------------------
// Local prototypes. Globals are declared in UI.h

void add_spacing(int spacing, int UI_size);
void print_long_line(int ui_size);
void print_long_line_pipe(int ui_size);
void print_long_line_equals(int ui_size);

// Garbage collector. Keeps reading the input until a new line (or the end of the file) is reached
void clear_input(void);

// Draws a UI in the terminal
void drawMenu(void);

// Processes the user input
void menu_choice(int node_count, station_list_node* list_of_stations, struct Graph* graph, float distance[], int previous_node[], float new_dist[], float average[], float percent[], float delays[]);

void list_all_stations(station_list_node* list_of_stations);

// Run dijkstra and its associated functions
void path_finder(int node_count, station_list_node* list_of_stations, struct Graph* graph, float dist[], int prev[], float new_dist[], float average[], float percent[], float delays[]);

void print_fine_Graph(struct Graph* graph, station_list_node* list_of_stations);


// -----------------------------------------------------------------------------------

// Garbage collector.
void clear_input(void){
    int garbageCollector;
    while ((garbageCollector = getchar()) != '\n' && garbageCollector != EOF)   // Keeps reading the input until a new line (or the end of the file) is reached
    {}
}

void add_spacing(int spacing, int UI_size)
{
    char drawMenu = '|';
    char menuSpacing = ' ';

    for (int i = 0; i < spacing; ++i) {
        printf("\n%c", drawMenu);     // Add space between text
        for (int j = 0; j < UI_size - 2; ++j) {
            printf("%c", menuSpacing);
        }
        printf("%c", drawMenu);
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
    char menuSpacing = ' ';

    print_long_line_equals(UI_SIZE);

    add_spacing(SPACING - 1, UI_SIZE);

    printf("\n%c%12cEuropean railway network simulation%13c", drawMenu, menuSpacing, drawMenu);
    printf("\n%c%4cMade by Emil, Eva, Frederik, Louise, Mads & Mohamad%5c", drawMenu, menuSpacing, drawMenu);

    add_spacing(SPACING - 1, UI_SIZE);

    print_long_line_pipe(UI_SIZE);

    add_spacing(SPACING - 1, UI_SIZE);

    printf("\n%c [r] Run simulation (placeholder)%27c", drawMenu, drawMenu);
    printf("\n%c [s] See all available stations%29c", drawMenu, drawMenu);
    printf("\n%c [g] Print graph (raw)%38c", drawMenu, drawMenu);
    printf("\n%c [f] Print graph (fine)%37c", drawMenu, drawMenu);
    printf("\n%c [q] Exit%51c", drawMenu, drawMenu);

    add_spacing(SPACING - 1, UI_SIZE);

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
void menu_choice(int node_count, station_list_node* list_of_stations, struct Graph* graph, float distance[], int previous_node[], float new_dist[], float average[], float percent[], float delays[])
{
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
                printf("\n");
                print_long_line_equals(UI_SIZE);
                printf("\n%c%21cRunning simulation%21c", draw_Menu, menuSpacing, draw_Menu);
                print_long_line_equals(UI_SIZE);
                printf("\n");

                path_finder(node_count, list_of_stations, graph, distance, previous_node, new_dist, average, percent, delays);

                printf("\n");
                print_long_line_equals(UI_SIZE);
                printf("\n%c%20cSimulation finished%21c", draw_Menu, menuSpacing, draw_Menu);
                print_long_line_equals(UI_SIZE);
                printf("\n\n");

                drawMenu();
                break;
            case 's':                           // Show all stations
                list_all_stations(list_of_stations);
                printf("\n\n");
                drawMenu();
                break;
            case 'g':                           // Print graph (raw)
                printf("\n");
                print_long_line_equals(LARGE_UI_SIZE);
                printf("\n%c%31cPrinted adjacency list%32c", draw_Menu, menuSpacing, draw_Menu);
                print_long_line_equals(LARGE_UI_SIZE);
                printf("\n");

                printGraph(graph);

                printf("\n");
                print_long_line_equals(LARGE_UI_SIZE);
                printf("\n%c%38cList end%39c", draw_Menu, menuSpacing, draw_Menu);
                print_long_line_equals(LARGE_UI_SIZE);
                printf("\n\n");

                drawMenu();
                break;
            case 'f':                             // Print graph (fine)
                printf("\n");
                print_long_line_equals(LARGE_UI_SIZE);
                printf("\n%c%32cStation connections%34c", draw_Menu, menuSpacing, draw_Menu);
                print_long_line_equals(LARGE_UI_SIZE);
                printf("\n");

                print_fine_Graph(graph, list_of_stations);

                print_long_line_equals(LARGE_UI_SIZE);
                printf("\n%c%38cList end%39c", draw_Menu, menuSpacing, draw_Menu);
                print_long_line_equals(LARGE_UI_SIZE);
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
    print_long_line_equals(LARGE_UI_SIZE);
    printf("\n%c%27cHere are all available stations%27c", draw_Menu, menuSpacing, draw_Menu);
    add_spacing(SPACING - 1, LARGE_UI_SIZE);
    printf("\n%c%17cStations from the following countries are included:%17c", draw_Menu, menuSpacing, draw_Menu);
    printf("\n%c%4cGermany, France, the Netherlands, Spain, Belgium, Italy & the Czech Republic%5c", draw_Menu, menuSpacing, draw_Menu);
    print_long_line_equals(LARGE_UI_SIZE);
    printf("\n");

    // Dynamically prints ALL available stations and their index.
    for (int i = 0; i <= 9; ++i) {
        printf("\n [%d] %s", i,index_station_list(list_of_stations, i));
    }
    for (int i = 10; i < (list_length(list_of_stations)); ++i) {
        //printf("\n%c [%d] %s",drawMenu, i, routes[i].station_start);
        printf("\n[%d] %s", i,index_station_list(list_of_stations, i));
    }
    printf("\n");
    print_long_line_equals(UI_SIZE);
    printf("\n%c%23cEnd of stations%22c", draw_Menu, menuSpacing, draw_Menu);
    print_long_line_equals(UI_SIZE);
    printf("\n");
}

void path_finder(int node_count, station_list_node* list_of_stations, struct Graph* graph, float dist[], int prev[], float new_dist[], float average[], float percent[], float delays[])
{
    for (int i = 0; i < node_count; ++i) {
        dijkstra(graph, dist, prev, i, list_length(list_of_stations));

        printf("\nDIJKSTRA %d", i);

        /*for (int j = 0; j < node_count; ++j) {
            //printf("\n%f + %f", dist[j],(float)delay_optimised(graph, start_train , prev, i, j ));
        }*/

        for (int j = 0; j < node_count; ++j) {
            struct Node *ptr = graph->head[j];
            while (ptr != NULL && ptr->dest != prev[j]) {     //find the correct route to check for matching trains on
                ptr = ptr->next;
            }
            if(i != j) {
                new_dist[j] = dist[j] + (float) delay_optimised(graph, ptr->allowed_trains, prev, i, j);
                new_dist[i] = 0;

                delays[j] = (float) delay_optimised(graph, ptr->allowed_trains, prev, i, j);
                delays[i] = 0;

                // Error testing here

                //printf("\n[2 | %d] Delay optimised: %lf\n", j, (float)delay_optimised(graph, ptr->allowed_trains, prev, i, j));
                //printf("[2 | %d] Dist: %f\n", j, dist[j]);
                //r
                // printf("[2 | %d] New dist: %f\n", j, new_dist[j]);
                /*
                if(900 < (float) delay_optimised(graph, ptr->allowed_trains, prev, i, j)){
                    printf("Starting with: %s", ptr->allowed_trains);
                    printf("\nOptimised: %lf\n", (float) delay_optimised(graph, ptr->allowed_trains, prev, i, j));
                    printf("Not Optimised: %lf\n", (float) extra_delay(graph, j, i, prev, start_train));
                }
                 */
            }
        }
        average[i] = average_weight_difference(new_dist, dist, node_count);
        percent[i] =  percentage_weight_difference(new_dist, dist, node_count);
        printf("\n[%d] Average delay is %f", i, average_weight_difference(new_dist, dist, node_count));
        printf("\n[%d] Average extra time in percent %f", i,  percent[i]);

        printf("\n[%d] Highest delay: %f", i, highest_num(delays, node_count));
        printf("\n[%d] Lowest delay %f", i, lowest_num(delays, node_count));
        printf("\n[%d] Median delay: %f", i, median_finder(delays, node_count));
        printf("\n\n");
    }
    printf("\nAverage of averages: %lf\n", average_simple(average, node_count));
    printf("Average of averages in percent: %lf\n", average_simple(percent, node_count));
}

void print_fine_Graph(struct Graph* graph, station_list_node* list_of_stations)
{
    int i;
    char menuSpacing = ' ';

    char station[XL_DATA_SIZE];

    station[0] = '\0';

    printf("\n");
    for (i = 0; i < N; i++)
    {
        struct Node* ptr = graph->head[i];
        // print the current vertex and all its neighbors
        while (ptr != NULL)
        {
            strcat(station, "[Station %d] ");
            strcat(station, index_station_list(list_of_stations, i));
            strcat(station," -> ");
            strcat(station, index_station_list(list_of_stations, ptr->dest));

            //printf("\nConcat string:%s\n", station_1);

            if(i <= 9)
                printf("\n [Station %d] %s -> %s", i, index_station_list(list_of_stations, i) , index_station_list(list_of_stations, ptr->dest));
            else if(i >= 10)
                printf("\n[Station %d] %s -> %s", i, index_station_list(list_of_stations, i) , index_station_list(list_of_stations, ptr->dest));

            for (int j = 0; j < 56-strlen(station); ++j) {
                printf("%c", menuSpacing);
            }
            printf("(Weight: %d)", ptr->weight);

            memset(station, '\0', sizeof(station));

            ptr = ptr->next;
        }
        printf("\n");
    }
}


// Encapsulates all UI functions into one for ease of use
void GenerateUI(int node_count, station_list_node* list_of_stations, struct Graph* graph, float dist[], int prev[], float new_dist[], float average[], float percent[], float delays[])
{
    drawMenu();
    menu_choice(node_count, list_of_stations, graph, dist, prev, new_dist, average, percent, delays);
}