#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stations.h"
#include "trains.h"

// Maybe have a struct with stats for each train? Like acceleration/deceleration (accel/decel), train cap, top speed,  and so on

// Sources for IC4
    // https://web.archive.org/web/20121202210720/http://www.siemens.com/press/pool/de/events/2011/industry/mobility/2011-05-icx/siemens-icx-factsheet-e.pdf
    // https://en.wikipedia.org/wiki/ICE_4#cite_note-ICxFact-7

void track_speed();
// Prolly not gonna be a single function as the problem is way more complex for just one

// Holds data for [INSERT FUNCTION NAME] for all edges.


void track_gauges();
// See above

// Holds data for [INSERT FUNCTION NAME] for all edges.

void power();
// See above

// Holds data for [INSERT FUNCTION NAME] for all edges.

void stations();
// See above

// Holds data for [INSERT FUNCTION NAME] for all edges.


void signals();
// See above

// Holds data for [INSERT FUNCTION NAME] for all edges.

//fisk

double time(double train_accel, double max_speed, double distance, int track_speed);

int main(void)
{
    printf("%s\n",IC4.name);

    double travel_time = time(IC4.accel, IC4.max_speed, berlinHBF_parisNord, 9999);
    printf("Travel time from berlinHBF to parisNord - assuming stuff, not accurate: %lf s\n", travel_time);

    return EXIT_SUCCESS;
}


void track_speed()
{

}

void track_gauges()
{

}

void power()
{

}


void stations()
{

}


void signals()
{

}

void train_cap() //maybe as a struct with train stats like speed, accel...
{

}

double time(double accel, double max_speed, double distance, int track_speed)
{
    if(track_speed < max_speed) {
        max_speed = track_speed;
    }

    double time_max_speed = 0;

    // Since data about decel is hard to find, it is expected that accel = decel.

    // This accounts both for accel and decel
    time_max_speed = 2 * (max_speed/accel); // Unit: s

    // Accel
    // v = a * t + v_0

    // t = (v-v_0)/a

    // t = (sqrt(2 * a_0 * s - 2 * a_0 * s_0 + v_0^2) - v_0)/ a_0

    double time = distance/max_speed; // Unit: s

    // This simple solution subtracts the accel/decel time with total time. While it isn't 100% accurate it tries to make an estimate of the time taken to travel the given distance.
    double total_time = time - time_max_speed;

    return total_time;
}



