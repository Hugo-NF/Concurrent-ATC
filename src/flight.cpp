#include "../include/flight.h"

void flight::evaluate_message(flight* flight_ref, radio_message msg) {
    switch (msg.type) {
        case CHECK_IN: {
            break;
        }
        case CHECK_OUT: {
            break;
        }
        case TAXI_CLEARANCE: {
            break;
        }
        case DESCEND_CLEARANCE: {
            break;
        }
        case CLEARANCE_DENY: {
            break;
        }
        case CLIMBING_CLEARANCE: {
            break;
        }
        case TAKEOFF_CLEARANCE: {
            break;
        }
        case LANDING_CLEARANCE: {
            break;
        }
        case EXIT_HOLDING_CLEARANCE: {
            break;
        }
        case MAYDAY_ASSISTANCE: {
            break;
        }
        case PANPAN_ASSISTANCE: {
            break;
        }
        default: {
            break;
        }
    }
}

void* flight::run(void* thread_target) {
    flight* flight_obj = (flight *) thread_target;
    // Departing aircraft
    if (flight_obj->flight_phase == ON_GROUND) {
        printf("%s (%s - %s) is preparing to fly from %s to %s\n",
            flight_obj->callsign.c_str(), 
            flight_obj->airplane.type.c_str(),
            flight_obj->airplane.reg.c_str(),
            flight_obj->origin.c_str(),
            flight_obj->destination.c_str()
        );
        sleep(flight_obj->time_to_pushback);
    }
    // Arriving aircraft
    else {
        printf("%s (%s - %s) is inbound to %s from %s\n",
            flight_obj->callsign.c_str(), 
            flight_obj->airplane.type.c_str(),
            flight_obj->airplane.reg.c_str(),
            flight_obj->destination.c_str(),
            flight_obj->origin.c_str()
        );
    }

    pthread_exit(0);
}

int flight::load_from_json_value(json_value* value) {
    for(unsigned int flight_index = 0; flight_index < value->u.object.length; flight_index++) {
        switch (flight_index) {
        case 0: {
            this->origin = value->u.object.values[flight_index].value->u.string.ptr;
            break;
        } 
        case 1: {
            this->destination = value->u.object.values[flight_index].value->u.string.ptr;
            break;
        } 
        case 2: {
            this->airplane.type = value->u.object.values[flight_index].value->u.string.ptr;
            airplane.load_from_json(value->u.object.values[flight_index].value->u.string.ptr);
            break;
        } 
        case 3: {
            this->airplane.reg = value->u.object.values[flight_index].value->u.string.ptr;
            break;
        } 
        case 4: {
            this->flight_no = value->u.object.values[flight_index].value->u.string.ptr;
            break;
        } 
        case 5: {
            this->callsign = value->u.object.values[flight_index].value->u.string.ptr;
            break;
        } 
        case 6: {
            this->time_to_pushback = value->u.object.values[flight_index].value->u.integer;
            break;
        }
        case 7: {
            this->distance_to_tod = value->u.object.values[flight_index].value->u.dbl;
            break;
        } 
        case 8: {
            this->fob = value->u.object.values[flight_index].value->u.dbl;
            break;
        } 
        default:
            printf("Warning: Unexpected property %s in flight %s\n", value->u.object.values[flight_index].name, this->origin.c_str());
            break;
        }
    }

    return 0;
}

void flight::print_info() {
    printf("\tFlight number: %s\n", this->flight_no.c_str());
    printf("\tCallsign: %s\n", this->callsign.c_str());
    printf("\tOrigin: %s\n", this->origin.c_str());
    printf("\tDestination: %s\n", this->destination.c_str());
    printf("\tFlight phase: %s\n", flight_rules::get_flight_phase(this->flight_phase));
    printf("\tFOB: %.2lf tons\n", this->fob);
    printf("\tAircraft: %s\n", this->airplane.type.c_str());
    printf("\tRegistration: %s\n\n", this->airplane.reg.c_str());
}
