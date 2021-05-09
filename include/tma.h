#ifndef TMA_H
#define TMA_H

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <map>
#include <string>
#include <sys/stat.h>

#include "airport.h"
#include "json.h"
#include "radio.h"
#include "threadable.h"

class tma: public threadable {
private:
    
    std::string id;
    std::string description;
    double radio_frequency;
    radio radio_channel;
    std::map<std::string, airport> airports;
    std::map<std::string, int> flights_on_service;


public:

    explicit tma() = default;
    virtual ~tma() = default;

    void* run(void* thread_id);
    int load_from_json(const char* filename);
    void print_info();

};

#endif // TMA_H