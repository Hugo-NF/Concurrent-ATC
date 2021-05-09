#include "../include/airport.h"

void* airport::run(void* thread_id) {
    this->twr_radio.transmit("SBGR_TWR", "LA3419", "Alinha e decola, pista 09L", nullptr, TAKEOFF_CLEARANCE);
    sleep(20);
}

void airport::load_from_json(const char* filename) {

}

void airport::print_info() {
    printf("Airport loaded\n");
}