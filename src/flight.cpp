#include "../include/flight.h"

void* flight::run(void* thread_id) {

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

        if (this->distance_to_tod == 0 && this->time_to_pushback > 0) {
            this->flight_phase = ON_GROUND;
        }
        else {
            this->flight_phase = CRUISING;
        }

        airplane.load_from_json(this->airplane.type);
    }
}

void flight::print_info() {
    printf("\tFlight number: %s\n", this->flight_no.c_str());
    printf("\tCallsign: %s\n", this->callsign.c_str());
    printf("\tOrigin: %s\n", this->origin.c_str());
    printf("\tDestination: %s\n", this->destination.c_str());
    printf("\tFlight phase: %s\n", flight_rules::get_flight_phase(this->flight_phase));
    printf("\tFOB: %lf\n", this->fob);
    printf("\tAircraft: %s\n", this->airplane.type.c_str());
    printf("\tRegistration: %s\n", this->airplane.reg.c_str());
}
