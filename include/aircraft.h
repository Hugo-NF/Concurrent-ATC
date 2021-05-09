#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <pthread.h>
#include <unistd.h>

class aircraft {
public:
    long descent_spd;
    long approach_spd;
    long landing_spd;
    long takeoff_spd;
    long climb_spd;
    long cruise_spd;
    long takeoff_distance;
    long landing_distance;

    long current_speed;
    long current_alt;


    explicit aircraft(
        long descent_spd,
        long approach_spd,
        long landing_spd,
        long takeoff_spd,
        long climb_spd,
        long cruise_spd,
        long takeoff_distance,
        long landing_distance,
        long starting_speed,
        long starting_alt
    );
    virtual ~aircraft() = default;

    unsigned int calculate_next_waypoint(double distance, double factor);
};

#endif // AIRCRAFT_H