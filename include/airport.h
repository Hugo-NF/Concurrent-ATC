#ifndef AIRPORT_H
#define AIRPORT_H

#include <map>
#include <list>
#include <string>
#include <unistd.h>
#include <sys/stat.h>

#include "json.h"
#include "radio.h"
#include "runway.h"
#include "threadable.h"

class airport : public threadable {
private:
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

public:
    void* run(void* thread_id);
    void print_info();
    int load_from_json(const char* airport_icao);
    
    explicit airport() = default;
    virtual ~airport() = default;
};

#endif // AIRPORT_H