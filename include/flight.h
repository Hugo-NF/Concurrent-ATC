#ifndef FLIGHT_H
#define FLIGHT_H

#include "aircraft.h"
#include "threadable.h"

class flight: public threadable {
private:
    /* data */
public:
    void* run(void* thread_id);
    int load_from_json(const char* filename);
    void print_info();

    explicit flight() = default;
    virtual ~flight() = default;
};


#endif // FLIGHT_H
