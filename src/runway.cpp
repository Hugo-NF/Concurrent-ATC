#include "../include/runway.h"

bool runway::try_join_runway(const char* aircraft_id) {
    if (pthread_mutex_trylock(&this->is_available) == 0) {
        this->aircraft_using_runway = aircraft_id;

        printf("Aircraft %s is now using RWY %s\n", aircraft_id, this->id.c_str());
        return true; // Aircraft can join the runway;
    }

    // Runway is occupied
    return false;
}

void runway::leave_runway() {
    pthread_mutex_unlock(&this->is_available);
}