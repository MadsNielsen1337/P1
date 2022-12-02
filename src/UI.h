#ifndef P1_UI_H
#define P1_UI_H

// Garbage collector. Keeps reading the input until a new line (or the end of the file) is reached
void clear_input(void);

// Draws a UI in the terminal
void drawMenu(route* routes, station_list_node* list_of_stations);

// Processes the user input
int menu_choice(route* routes, station_list_node* list_of_stations);

// Encapsulates all UI functions into one for ease of use
void GenerateUI(route* routes, station_list_node* list_of_stations);

#endif //P1_UI_H
