#ifndef P1_STATIONS_H
#define P1_STATIONS_H
// Data taken from "https://www.raileurope.com/en"
// For more info see "https://www.raileurope.com/blog/travel-green-calculating-your-carbon-savings"

// When multiple routes for the same journey are available, the route with shortest travel time is selected.

// All stations are of format "SOURCE_DESTINATION".
// Unit is m.

enum station_e {Berlin_Hbf,
                Paris_Nord,
                Frankfurt_am_Main_Hbf,
                Bruxelles_Midi};


double berlinHBF_parisNord = 1000.52 * 1000;

#endif //P1_STATIONS_H
