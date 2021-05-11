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
    double descent_ff;
    double approach_ff;
    double takeoff_ff;
    double landing_ff;
    double climb_ff;
    double cruise_ff;
    double idle_ff;

    long current_speed;
    long current_alt;
    double current_ff;


    explicit aircraft()= default;
    virtual ~aircraft() = default;

    int load_from_json(std::string aircraft_type);
    double calculate_remaining_fuel(double fob, double time);
    unsigned int calculate_next_waypoint(double distance);
};

#endif // AIRCRAFT_H