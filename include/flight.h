#ifndef FLIGHT_H
#define FLIGHT_H

#include "aircraft.h"
#include "flight_rules.h"
#include "json.h"

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
   
    static void* run(void* thread_target);
    int load_from_json_value(json_value* value);
    void print_info();

    explicit flight() = default;
    virtual ~flight() = default;
};


#endif // FLIGHT_H
