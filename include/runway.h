#ifndef RUNWAY_H
#define RUNWAY_H

#include <string>
#include <pthread.h>

class runway {

    std::string id;
    std::string aircraft_using_runway;
    long length;
    long heading;
    pthread_mutex_t is_available = PTHREAD_MUTEX_INITIALIZER;

public:
    explicit runway(const char* id, long length, long heading) {
        this->id = id;
        this->length = length;
        this->heading = heading;
    }
    virtual ~runway() = default;

    bool try_join_runway(const char* aircraft_id);
    void leave_runway();
};

#endif // RUNWAY_H