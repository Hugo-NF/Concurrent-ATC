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

void runway::load_from_json_value(const char* airport_icao, json_value* value) {
    int obj_length = value->u.object.length;
    for (int x = 0; x < obj_length; x++) {
        switch (x) {
            case 0: {
                this->id = value->u.object.values[x].value->u.string.ptr;
                break;
            }
            case 1: {
                this->heading = value->u.object.values[x].value->u.integer;
                break;
            }
            case 2: {
                this->length = value->u.object.values[x].value->u.integer;
                break;
            }
            case 3: {
                printf("Apps");
                break;
            }
            case 4: {
                printf("Sids");
                break;
            }
            case 5: {
                printf("Stars");
                break;
            }
            default: {
                printf("Warning: Unexpected property %s in %s.json\n", value->u.object.values[x].name, airport_icao);
                break;
            }
        }
    }
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