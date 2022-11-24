#ifndef P1_TRAINS_H
#define P1_TRAINS_H


//for structs with train specs

// Units for structs:
// accel: m/s^2
// max_speed: m/s


// IC4 with 7 cars
// https://web.archive.org/web/20121202210720/http://www.siemens.com/press/pool/de/events/2011/industry/mobility/2011-05-icx/siemens-icx-factsheet-e.pdf
typedef struct {
    char name[10];
    double accel;
    double max_speed;
    int passenger_cap;


} train_s;

train_s IC4 = {"IC4", 0.55, 230/3.6, 499};



#endif //P1_TRAINS_H
