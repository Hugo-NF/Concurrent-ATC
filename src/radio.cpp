#include "../include/radio.h"

// Some one wants to speak
void radio::transmit(
    const char* sender,
    const char* recipient,
    const char* content,
    const char* args,
    message_types type
) {
    pthread_mutex_lock(&this->transmitting);
    this->mqueue.emplace_back(sender, recipient, content, args, type);
    pthread_mutex_unlock(&this->transmitting);
}

radio_message radio::listen(const char *recipient) {
    if (pthread_mutex_trylock(&this->transmitting) == 0) {
        // Radio available (lock acquired)
        if(mqueue.size() > 0) {
            for(auto it = mqueue.begin(); it != mqueue.end(); ++it) {
                if (it.operator*().recipient.compare(recipient) == 0) {
                    radio_message msg = it.operator*();
                    mqueue.erase(it);
                    
                    pthread_mutex_unlock(&this->transmitting);
                    return msg;
                }
            }
            
            pthread_mutex_unlock(&this->transmitting);
            // No messages to recipient
            return radio_message(); //blank message
        }
    }

    // Radio not available | no messages on frequency.
    return radio_message(); //blank message
}