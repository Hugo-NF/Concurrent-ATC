#include "../include/radio.h"

// Global radio frequencies
std::map<double, radio> frequencies = std::map<double, radio>();

// Some one wants to speak
void radio::transmit(
    const char* sender,
    const char* recipient,
    const char* content,
    void* arg,
    message_types type
) {
    pthread_mutex_lock(&this->transmitting);
    this->mqueue.emplace_back(sender, recipient, content, arg, type);
    pthread_mutex_unlock(&this->transmitting);
}

radio_message radio::listen(const char *recipient) {
    pthread_mutex_lock(&this->transmitting);
    // Radio available (lock acquired)
    if (mqueue.size() > 0) {
        for(auto it = mqueue.begin(); it != mqueue.end(); ++it) {
            if (it.operator*().recipient.compare(recipient) == 0) {
                radio_message msg = it.operator*();
                mqueue.erase(it);
                
                pthread_mutex_unlock(&this->transmitting);
                return msg;
            }
        }
    }
    pthread_mutex_unlock(&this->transmitting);
    
    // No messages to recipient
    // Radio not available | no messages on frequency.
    return radio_message(); //blank message
}