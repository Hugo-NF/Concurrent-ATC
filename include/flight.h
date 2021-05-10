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
    
    airport* target_airport;

    sid_star* current_procedure;
    approach* current_approach;
    runway* current_runway;

    aircraft airplane;
   
    int load_from_json_value(json_value* value);
    void print_info();

    static void evaluate_message(flight* flight_ref, radio_message msg);
    static void disconnect_radio(int signal); 
    static void* run(void* thread_target);

    explicit flight() = default;
    virtual ~flight() = default;
};


#endif // FLIGHT_H
