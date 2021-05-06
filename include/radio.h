#include <pthread.h>
#include <list>
#include <string>

#include "flight_rules.h"

using namespace std;

class radio_message {
public:
    bool blank;
    string sender;
    string recipient;
    string content;
    message_types type;

    explicit radio_message(
        const char* sender,
        const char* recipient,
        const char* content,
        message_types type  
    ) {
        this->sender = sender;
        this->recipient = recipient;
        this->content = content;
        this->type = type;
        this->blank = false;
    }

    radio_message() {
        this->blank = true;
    }
};

class radio {

private:
    pthread_mutex_t transmitting = PTHREAD_MUTEX_INITIALIZER;
    list<radio_message> mqueue;

public:
    explicit radio() = default;
    virtual ~radio() = default;

    // Some one wants to speak
    void transmit(const char* sender, const char* recipient, const char* content, message_types type);
    radio_message listen(const char *recipient);
};