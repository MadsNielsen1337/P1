#ifndef P1_TRAINS_H
#define P1_TRAINS_H

//for structs with train specs

// Units for structs:
// accel: m/s^2
// max_speed: m/s

enum fuels_e {diesel, el_15kV_16Hz, el_25kV_50Hz};


typedef struct {
    char name[20],
         track_gauge[20],
         control[20],
         fuel_type[20];
    double accel,
           max_speed;
    int passenger_cap;
} train_s;

// IC4 with 7 cars
// Sources for IC4
// https://web.archive.org/web/20121202210720/http://www.siemens.com/press/pool/de/events/2011/industry/mobility/2011-05-icx/siemens-icx-factsheet-e.pdf
// https://en.wikipedia.org/wiki/ICE_4#cite_note-ICxFact-7
train_s IC4 = {"IC4","Standard", "ETCS.LZB.PZB", "15kV_16Hz.25kV_50Hz", 0.55, 230/3.6, 499};



#endif //P1_TRAINS_H
