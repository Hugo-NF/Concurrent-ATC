#ifndef RADIO_H
#define RADIO_H

#include <list>
#include <map>
#include <pthread.h>

#include "radio_message.h"

class radio {

public:
    pthread_mutex_t transmitting = PTHREAD_MUTEX_INITIALIZER;

    std::list<radio_message> mqueue;

    explicit radio() = default;
    virtual ~radio() = default;

    // Some one wants to speak
    void transmit(const char* sender, const char* recipient, const char* content, void* arg, message_types type);
    radio_message listen(const char *recipient);
};

extern std::map<double, radio> frequencies;

#endif // RADIO_H