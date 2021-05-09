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
    aircraft_using_runway.clear();
    pthread_mutex_unlock(&this->is_available);
}

void runway::load_from_json_value(json_value* value) {
    printf("Loading runway...");
    this->id = "09L";
}

void runway::print_info() {
    printf("\t\t\tID: %s\n", this->id.c_str());
    printf("\t\t\tHeading: %ld\n", this->heading);
    printf("\t\t\tLength (ft): %ld\n", this->length);
    printf("\t\t\tSIDS: \n");
    // TODO: Print SIDS
    printf("\t\t\tSTARS: \n");
    // TODO: Print STARS
    printf("\t\t\tAPPROACHES: \n");
    // TODO: Print APPS
}