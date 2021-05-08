#ifndef AIRPORT_H
#define AIRPORT_H

#include <unistd.h>

#include "threadable.h"
#include "radio.h"

class airport : public threadable {
private:
    radio twr_radio;

public:
    radio* get_radio_instance();
    void* run(void* thread_id);

    explicit airport() = default;
    virtual ~airport() = default;
};

#endif // AIRPORT_H