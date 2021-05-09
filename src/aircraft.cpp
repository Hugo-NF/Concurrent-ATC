#include "../include/aircraft.h"

aircraft::aircraft(
    std::string type,
    std::string reg,
    long descent_spd,
    long approach_spd,
    long landing_spd,
    long takeoff_spd,
    long climb_spd,
    long cruise_spd,
    long takeoff_distance,
    long landing_distance,
    long descent_ff,
    long approach_ff,
    long takeoff_ff,
    long climb_ff,
    long cruise_ff,
    long idle_ff,
    long starting_speed,
    long starting_alt,
    long starting_ff
) {
    this->type = type;
    this->reg = reg;
    this->descent_spd = descent_spd;
    this->approach_spd = approach_spd;
    this->landing_spd = landing_spd;
    this->takeoff_spd = takeoff_spd;
    this->climb_spd = climb_spd;
    this->cruise_spd = cruise_spd;
    this->takeoff_distance = takeoff_distance;
    this->landing_distance = landing_distance;
    this->descent_ff = descent_ff;
    this->approach_ff = approach_ff;
    this->takeoff_ff = takeoff_ff;
    this->climb_ff = climb_ff;
    this->cruise_ff = cruise_ff;
    this->idle_ff = idle_ff;
    this->current_speed = starting_speed;
    this->current_alt = starting_alt;
    this->current_ff = starting_ff;
}

int load_from_json(std::string aircraft_type) {
    printf("Loading aircraft: %s\n", aircraft_type.c_str());
    return 0;
}

double aircraft::calculate_remaining_fuel(double fob, double time) {
    double result = (time * this->current_ff) / 3600;
    return fob - result;
}

unsigned int aircraft::calculate_next_waypoint(double distance) {
    double result = (distance * 3600) / this->current_speed;
    return (unsigned int) floor(result);
}