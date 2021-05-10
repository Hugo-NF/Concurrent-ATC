#ifndef TMA_H
#define TMA_H

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <list>
#include <map>
#include <string>
#include <sys/stat.h>

#include "airport.h"
#include "flight.h"
#include "json.h"
#include "radio.h"

class tma {
public:
    
    std::string id;
    std::string description;
    double radio_frequency;
    radio radio_channel;

    std::map<std::string, airport> airports;
    
    
    std::map<std::string, int> flights_on_service;
    std::list<flight> flights_on_terminal; 


    explicit tma() = default;
    virtual ~tma() = default;

    int load_from_json(const char* filename);
    int load_flights(const char* filename);
    void print_info();

    static void evaluate_message(tma* tma_ref, radio_message msg);
    static void* run(void* thread_id);

};

#endif // TMA_H