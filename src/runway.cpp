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
                auto array = value->u.object.values[x].value->u.array;
                for (unsigned int x = 0; x < array.length; x++) {
                    std::string current_approach = array.values[x]->u.string.ptr;
                    approach new_approach = approach();
                    new_approach.load_from_json(current_approach);
                    this->approaches[current_approach] = new_approach;
                }
                break;
            }
            case 4: {
                auto array = value->u.object.values[x].value->u.array;
                for (unsigned int x = 0; x < array.length; x++) {
                    std::string current_sid = array.values[x]->u.string.ptr;
                    sid_star new_sid = sid_star();
                    new_sid.load_from_json(current_sid);
                    this->sids[current_sid] = new_sid;
                }
                break;
            }
            case 5: {
                auto array = value->u.object.values[x].value->u.array;
                for (unsigned int x = 0; x < array.length; x++) {
                    std::string current_star = array.values[x]->u.string.ptr;
                    sid_star new_star = sid_star();
                    new_star.load_from_json(current_star);
                    this->stars[current_star] = new_star;
                }
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

    printf("\t\t\tSIDS: ");
    for(auto it = sids.begin(); it != sids.end(); ++it) {
        if (std::distance(it, sids.end()) == 1) {
            printf("%s", it.operator*().first.c_str());
            continue;
        }
        printf("%s, ", it.operator*().first.c_str());
    }
    
    printf("\n\t\t\tSTARS: ");
    for(auto it = stars.begin(); it != stars.end(); ++it) {
        if (std::distance(it, stars.end()) == 1) {
            printf("%s", it.operator*().first.c_str());
            continue;
        }
        printf("%s, ", it.operator*().first.c_str());
    }

    printf("\n\t\t\tAPPROACHES: ");
    for(auto it = approaches.begin(); it != approaches.end(); ++it) {
        if (std::distance(it, approaches.end()) == 1) {
            printf("%s", it.operator*().first.c_str());
            continue;
        }
        printf("%s, ", it.operator*().first.c_str());
    }
    printf("\n\n");
}