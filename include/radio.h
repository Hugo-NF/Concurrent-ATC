#ifndef RADIO_H
#define RADIO_H

#include <pthread.h>
#include <list>

#include "radio_message.h"

class radio {

private:
    pthread_mutex_t transmitting = PTHREAD_MUTEX_INITIALIZER;
    std::list<radio_message> mqueue;

public:
    explicit radio() = default;
    virtual ~radio() = default;

    // Some one wants to speak
    void transmit(const char* sender, const char* recipient, const char* content, void* arg, message_types type);
    radio_message listen(const char *recipient);
};

#endif // RADIO_H