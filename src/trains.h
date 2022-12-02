#ifndef P1_TRAINS_H
#define P1_TRAINS_H
#define TRAIN_COUNT 3

typedef struct train{
    char name[DATA_SIZE];
    char gauge[DATA_SIZE];
    char controls[DATA_SIZE];
    char fuels[DATA_SIZE];
    double acceleration;
    double max_speed;
    int passenger_space;
} train;

void scan_trains(FILE* p_file, train* t){   //reads from a file where each line has the following format: Name,Gauge,Control1.Control2.Control3,Fuel1.Fuel2.Fuel3,Acceleration,Max_speed,Passenger_space
    char temp[DATA_SIZE];
    for (int i = 0; i < TRAIN_COUNT; ++i) {

        fscanf(p_file, "%[^,]", t[i].name);
        fscanf(p_file, "%*[,]");
        fscanf(p_file, "%[^,]", t[i].gauge);
        fscanf(p_file, "%*[,]");
        fscanf(p_file, "%[^,]", t[i].controls);
        fscanf(p_file, "%*[,]");
        fscanf(p_file, "%[^,]", t[i].fuels);
        fscanf(p_file, "%*[,]");
        fscanf(p_file, "%[^,]", temp);
        t[i].acceleration = strtod(temp, NULL);        //converting the string to a double, so we can do math on it
        fscanf(p_file, "%*[,]");
        fscanf(p_file, "%[^,]", temp);
        t[i].max_speed = strtod(temp, NULL);
        fscanf(p_file, "%*[,]");
        fscanf(p_file, "%[^\n]", temp);
        t[i].passenger_space = strtol(temp, NULL, 10);
        fscanf(p_file, "%*[^a-zA-Z0-9]");

        //printf to test if the format is right. delete later
        printf("%s %s %s %s %.0lf %.0lf %d\n",t[i].name,t[i].gauge,t[i].controls,t[i].fuels,t[i].acceleration,t[i].max_speed,t[i].passenger_space);
    }
}

#endif //P1_TRAINS_H
