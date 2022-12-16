#ifndef P1_UI_H
#define P1_UI_H

// Encapsulates all UI functions into one for ease of use
void GenerateUI(int node_count, station_list_node* list_of_stations, struct Graph* graph, float distance[], int previous_node[], float new_dist[], float average[], float percent[]);

#endif //P1_UI_H
