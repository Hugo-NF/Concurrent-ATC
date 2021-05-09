#include "../include/aircraft.h"

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