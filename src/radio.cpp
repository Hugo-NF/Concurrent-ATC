#include "../include/radio.h"

// Some one wants to speak
void radio::transmit(
    const char* sender,
    const char* recipient,
    const char* content,
    message_types type
) {
    pthread_mutex_lock(&this->transmitting);
    this->mqueue.emplace_back(sender, recipient, content, type);
    pthread_mutex_unlock(&this->transmitting);
}

message radio::listen(const char *recipient) {
    if(mqueue.size() > 0) {
        if (pthread_mutex_trylock(&this->transmitting)) {
            // Radio available (lock acquired)

            for(auto it = mqueue.begin(); it != mqueue.end(); ++it) {
                if(it.operator*().recipient.compare(recipient)) {
                    it = mqueue.erase(it);

                    pthread_mutex_unlock(&this->transmitting);
                    return it.operator*();
                }
            }
            
            pthread_mutex_unlock(&this->transmitting);
            // No messages to recipient
            return message(); //blank message
        }
    }

    // Radio not available | no messages on frequency.
    return message(); //blank message
}