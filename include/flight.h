#ifndef FLIGHT_H
#define FLIGHT_H

#include <unistd.h>

#include "aircraft.h"
#include "airport.h"
#include "flight_rules.h"
#include "json.h"
#include "radio_message.h"

class flight {
public: 
    std::string origin;
    std::string destination;
    std::string flight_no;
    std::string callsign;

    int flight_phase;
    double fob;

    long time_to_pushback;
    double distance_to_tod;

    double current_radio_frequency;
    radio* current_radio_channel;
    
    airport* target_airport;

    sid_star* current_procedure;
    approach* current_approach;
    runway* current_runway;
    unsigned int current_waypoint;

    aircraft airplane;
   
    int load_from_json_value(json_value* value);
    void print_info();

    static void evaluate_message(flight* flight_ref, radio_message msg);
    static void* run(void* thread_target);

    explicit flight() = default;
    virtual ~flight() = default;
};


#endif // FLIGHT_H
