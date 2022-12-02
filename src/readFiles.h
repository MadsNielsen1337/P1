#ifndef P1_READFILES_H
#define P1_READFILES_H

// Counts the number of lines in the file pointed to by p_file. Uses up the entire input stream, so if the file needs to be used again it must be closed and reopened!
int lines_in_file(FILE* p_file);

// Reads input files in *.txt using scan_routes & scan_trains
void read_routes(route* routes, int* route_count);
void read_trains(train* trains, int* train_count);


// Reads from a file where each line has the following format: Distance,Track_speed,Power,Gauge,Controls,Station_Start,Station_End
void scan_routes(FILE* p_file, route* r, int* route_count);

// Reads from a file where each line has the following format: Name,Track_gauge,Control1.Control2.Control3,Power1.Power2.Power3,Acceleration,Max_speed,Passenger_cap
void scan_trains(FILE* p_file, train* t, int* train_count);

// Calculates size of *.txt files - aka. how many lines are there?
int get_route_size(int route_count);
int get_train_size(int train_count);
#endif //P1_READFILES_H
