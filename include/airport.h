#ifndef AIRPORT_H
#define AIRPORT_H

#include <errno.h>                                                              
#include <pthread.h>
#include <signal.h>
#include <map>
#include <list>
#include <string>
#include <unistd.h>
#include <sys/stat.h>

#include "json.h"
#include "radio.h"
#include "runway.h"

class airport {
public:
    // Data
    std::string icao_id;
    std::string iata_id;
    std::string name;
    long elevation_ft;

    // Comms
    double radio_frequency;
    radio radio_channel;
    std::map<std::string, int> flights_on_service;
    
    // Runways
    std::map<std::string, runway> runways;


    void print_info();
    int load_from_json(const char* airport_icao);
    
    static void evaluate_message(airport* airport_ref, radio_message msg);
    static void finish_operation(int signal); 
    static void* run(void* thread_target);

    explicit airport() = default;
    virtual ~airport() = default;
};

#endif // AIRPORT_H