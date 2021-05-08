#ifndef AIRPORT_H
#define AIRPORT_H

#include <list>
#include <string>
#include <unistd.h>

#include "radio.h"
#include "runway.h"
#include "threadable.h"

class airport : public threadable {
private:
    // Identifiers
    std::string icao_id;
    std::string name;
    int elevation_ft;

    // Comms
    int twr_frequency;
    radio twr_radio;

    // Runways
    std::list<runway> runways;

public:
    radio* get_radio_instance();
    void* run(void* thread_id);

    explicit airport() = default;
    virtual ~airport() = default;
};

#endif // AIRPORT_H