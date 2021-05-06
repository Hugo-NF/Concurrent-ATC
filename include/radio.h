#include <pthread.h>
#include <list>
#include <string>

#include "flight_rules.h"

using namespace std;

class message {
public:
    bool blank;
    string sender;
    string recipient;
    string content;
    message_types type;

    explicit message(
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

    message() {
        this->blank = true;
    }
};

class radio {

private:
    pthread_mutex_t transmitting = PTHREAD_MUTEX_INITIALIZER;
    list<message> mqueue;

public:
    explicit radio() = default;
    virtual ~radio() = default;

    // Some one wants to speak
    void transmit(const char* sender, const char* recipient, const char* content, message_types type);
    message listen(const char *recipient);
};