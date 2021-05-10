#ifndef FLIGHT_H
#define FLIGHT_H

#include <unistd.h>

#include "aircraft.h"
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

    aircraft airplane;
   
    int load_from_json_value(json_value* value);
    void print_info();

    static void evaluate_message(flight* flight_ref, radio_message msg);
    static void* run(void* thread_target);

    explicit flight() = default;
    virtual ~flight() = default;
};


#endif // FLIGHT_H
