#ifndef RADIO_MESSAGE_H
#define RADIO_MESSAGE_H

#include <string>

#include "flight_rules.h"

class radio_message {
public:
    bool blank;
    std::string sender;
    std::string recipient;
    std::string content;
    void* arg;
    message_types type;

    explicit radio_message(
        const char* sender,
        const char* recipient,
        const char* content,
        void* arg,
        message_types type  
    ) {
        this->sender = sender;
        this->recipient = recipient;
        this->content = content;
        this->arg = arg;
        this->type = type;
        this->blank = false;
    }

    radio_message() {
        this->blank = true;
    }
};

#endif // RADIO_MESSAGE_H