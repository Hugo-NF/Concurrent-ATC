#include "../include/airport.h"

radio* airport::get_radio_instance() {
    return &this->twr_radio;
}

void* airport::run(void* thread_id) {
    
    this->twr_radio.transmit("SBGR_TWR", "LA3419", "Alinha e decola, pista 09L", "09L", TAKEOFF_CLEARANCE);
    sleep(20);
}