#include <math.h>
#include "routes.h"
#include "trains.h"
#include "time_calc.h"

#define METER_CONVERSION 1000               // Multiply this with km to get m
#define METER_PER_SECOND_CONVERSION 3.6     // Divide this with km/h to get m/s

// Calculate the time it takes to go from station_start to station_end. Takes accel & decel into account
double time(route r, train t)
{
    // Since data about decel is hard to find, it is expected that accel = decel.

    // Accel
    // v = a * t + v_0

    // t = (v-v_0)/a

    // Include the distance
    // t = sqrt(2 * a * s + v_0^2) - v_0 / a

    if(r.track_speed < t.max_speed)
        t.max_speed = r.track_speed;

    double time_max_speed,
           time_max_speed_half_distance,
           s,
           time,
           average_speed_half,
           final_time_large,
           final_time;

    t.max_speed /= METER_PER_SECOND_CONVERSION;
    r.distance *= METER_CONVERSION;

    // The time it takes to accelerate from v = 0 to t.max_speed
    time_max_speed = t.max_speed / t.acceleration; // Unit: s

    // s = 1/2 * a * t^2 + v_0 * t
    // The distance travelled with t.accel and time_max_speed starting from v = 0
    s = 1.0/2.0 * t.acceleration * pow(time_max_speed, 2);

    // Include the distance
    // t = sqrt(2 * a * s + v_0^2) - v_0 / a
    // The time it takes to travel the accel distance with t.accel.
    time_max_speed_half_distance = sqrt(2 * t.acceleration * s) / t.acceleration;

    // t = s / v_0  ->  t * v_0 = s  ->  v_0 = s / t
    // The average speed of half of t.accel distance. This corresponds to t.max_speed in the accel/decel phase
    average_speed_half = (s / 2.0) / (time_max_speed_half_distance / 2);

    if(s > r.distance/2.0) {
        // The time it takes to travel the total distance (which is the accel/decel distance) with the average accel/decel speed.
        final_time = (r.distance) / average_speed_half;

        return final_time;
    }
    else {
        // The time it takes to travel the accel/decel distance with the average accel/decel speed.
        final_time_large = (2 * s) / average_speed_half;

        // The time it takes to travel r.distance. With greater precision since accel/decel calculations have been refined
        time = ((r.distance - (2 * s)) / t.max_speed) + final_time_large; // Unit: s

        return time;
    }
}

// Calculates the weight for the edge going from station_start to station_end
int weight_calc(route r, train t)
{
    int weight;    // Delay time in minutes

    weight = time(r, t);
    // maybe also make a delay at each vertex to account for passengers leaving/getting on the train - unless the vertex is the final destination

    if(weight < 0) {
        return -1;
    }

    return weight;
}