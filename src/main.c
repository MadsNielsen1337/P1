#include <stdio.h>
#include <stdlib.h>
#include "stations.h"

// Maybe have a struct with stats for each train? Like acceleration/deceleration (accel/decel) (which calcs average speed), train cap, top speed,  and so on
    // Would make it more accurate

// Sources for IC4
    // https://web.archive.org/web/20121202210720/http://www.siemens.com/press/pool/de/events/2011/industry/mobility/2011-05-icx/siemens-icx-factsheet-e.pdf
    // https://en.wikipedia.org/wiki/ICE_4#cite_note-ICxFact-7

void distance();
void track_speed();
void track_gauges();
void power();
void stations();
void signals();
void train_cap();


int main(void)
{
    printf("Fisk\n");
    distance();
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



void distance() {
    printf("%lf", berlinHBF_parisNord);
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





