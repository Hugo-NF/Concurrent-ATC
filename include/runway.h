#ifndef RUNWAY_H
#define RUNWAY_H

#include <iterator>
#include <map>
#include <string>
#include <pthread.h>

#include "json.h"
#include "waypoint.h"

class runway {
private:
    pthread_mutex_t is_available = PTHREAD_MUTEX_INITIALIZER;

public:
    std::string id;
    std::string aircraft_using_runway;
    long length;
    long heading;

    // Procedures
    std::map<std::string, sid_star> sids;
    std::vector<std::string> active_sids;

    std::map<std::string, sid_star> stars;
    std::vector<std::string> active_stars;

    std::map<std::string, approach> approaches;
    std::vector<std::string> active_approaches;


    explicit runway() = default;
    virtual ~runway() = default;

    bool try_join_runway(const char* aircraft_id);
    void leave_runway();
    void load_from_json_value(const char* airport_icao, json_value* value);
    void print_info();
};

#endif // RUNWAY_H