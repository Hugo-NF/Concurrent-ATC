#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <string>
#include <sys/stat.h>


#include "json.h"

class aircraft {
public:
    std::string type;
    std::string reg;
    long descent_spd;
    long approach_spd;
    long landing_spd;
    long takeoff_spd;
    long climb_spd;
    long cruise_spd;
    long takeoff_distance;
    long landing_distance;
    long descent_ff;
    long approach_ff;
    long takeoff_ff;
    long climb_ff;
    long cruise_ff;
    long idle_ff;

    long current_speed;
    long current_alt;
    long current_ff;


    explicit aircraft()= default;
    virtual ~aircraft() = default;

    int load_from_json(std::string aircraft_type);
    double calculate_remaining_fuel(double fob, double time);
    unsigned int calculate_next_waypoint(double distance);
};

#endif // AIRCRAFT_H