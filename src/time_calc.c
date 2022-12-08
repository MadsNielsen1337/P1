#include "stdio.h"
#include "routes.h"
#include "trains.h"
#include "time_calc.h"

#define METER_CONVERSION 1000               // Multiply this with km to get m
#define METER_PER_SECOND_CONVERSION 3.6     // Divide this with km/h to get m/s

// Calculate the time it takes to go from station_start to station_end. Takes accel & decel into account
double time(route r, train t)
{
    if(r.track_speed < t.max_speed)
        t.max_speed = r.track_speed;


    double time_max_speed;
    t.max_speed /= METER_PER_SECOND_CONVERSION;
    r.distance *= METER_CONVERSION;

    // Since data about decel is hard to find, it is expected that accel = decel.
    // This accounts both for accel and decel

    // Accel
    // v = a * t + v_0

    // t = (v-v_0)/a

    // t = (sqrt(2 * a_0 * s - 2 * a_0 * s_0 + v_0^2) - v_0)/ a_0


    // The time it takes to accelerate from v = 0 to t.max_speed (Times 2 to account for decel)
    time_max_speed = 2 * ((t.max_speed) / t.acceleration); // Unit: s


    double v;
    // Velocity at time_max_speed
    v = t.acceleration * time_max_speed / 2;

    // The time it takes to travel distance with velocity v
    double time = (r.distance) / t.max_speed; // Unit: s

    // This simple solution subtracts the accel/decel time with total time. While it isn't 100% accurate it tries to make an estimate of the time taken to travel the given distance.
    double total_time = (time - time_max_speed) + added_delay(r, t);

    if(time_max_speed > total_time) {
        total_time = (time - (time_max_speed / 2)) + added_delay(r, t);
    }

    return total_time;
}

// Adds delay to the time if we're switching trains. More or less a placeholder for now
double added_delay(route r, train t)
{
    double delay = 0;    // Delay time in minutes

    if(t.fuels != r.power)
        delay += 5;
    if(t.gauge != r.gauge)
        delay += 5;
    if(t.controls != r.controls)
        delay += 5;

    return delay;

}

// Calculates the weight for the edge going from station_start to station_end
int weight_calc(route r, train t)
{
    int weight = 0;    // Delay time in minutes

    weight += time(r, t);
    // maybe also make a delay at each vertex to account for passengers leaving/getting on the train - unless the vertex is the final destination


    // Comments down below are loose thoughts and random stuff


    // We convert km && km/h to m && m/s
    //   weight += ((distance * METER_CONVERSION) / (avg_track_speed_r/METER_PER_SECOND_CONVERSION)); // distance travel time with average track speed.

    // check to see if the train in use can continue onto the next edge. Find a way to check layer 3 & 4 stats for next edge.
    /*if(power_f != fuel_type_s || track_gauge_f != train_track_gauge_s || control_f != control_s)             // Doubt you can do this. Prolly need strcmp or something
        weight += 10;       // Added delay in minutes. Find the average time it takes to switch trains.
*/

    return weight;
}