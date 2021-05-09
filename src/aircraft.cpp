#include "../include/aircraft.h"

aircraft::aircraft(
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
) {
    this->descent_spd = descent_spd;
    this->approach_spd = approach_spd;
    this->landing_spd = landing_spd;
    this->takeoff_spd = takeoff_spd;
    this->climb_spd = climb_spd;
    this->cruise_spd = cruise_spd;
    this->takeoff_distance = takeoff_distance;
    this->landing_distance = landing_distance;
    this->current_speed = starting_speed;
    this->current_alt = starting_alt;
}

unsigned int aircraft::calculate_next_waypoint(double distance, double factor) {
    double result = (distance * 3600) / this->current_speed;
    return (unsigned int) floor(result * factor);
}