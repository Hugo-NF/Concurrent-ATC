#include "../include/flight.h"

void flight::evaluate_message(flight* flight_ref, radio_message msg) {
    printf("[%s] %s", msg.sender.c_str(), msg.content.c_str());
    
    char message_text[MESSAGE_SIZE];
    std::string message_buff;

    switch (msg.type) {
        case CHECK_IN: {
            if(flight_ref->flight_phase == CRUISING) {
                flight_ref->target_airport = (airport *) msg.arg;

                snprintf(
                    message_text,
                    MESSAGE_SIZE,
                    "%s para %s, ciente. %.2lf milhas para o ideal de descida\n",
                    msg.recipient.c_str(),
                    msg.sender.c_str(),
                    flight_ref->distance_to_tod
                );
                message_buff = message_text;

                frequencies[flight_ref->current_radio_frequency].transmit(
                    flight_ref->callsign.c_str(), 
                    frequencies[flight_ref->current_radio_frequency].callsign.c_str(),
                    message_buff.c_str(),
                    NULL,
                    DESCEND_REQUEST
                );
                unsigned int distance_time = flight_ref->airplane.calculate_next_waypoint(flight_ref->distance_to_tod);
                flight_ref->fob = flight_ref->airplane.calculate_remaining_fuel(flight_ref->fob, distance_time);
                sleep(distance_time);
            }
            break;
        }
        case CHECK_OUT: {
            break;
        }
        case TAXI_CLEARANCE: {
            break;
        }
        case DESCEND_CLEARANCE: {
            if(flight_ref->flight_phase == CRUISING) {
                flight_ref->current_procedure = (sid_star *) msg.arg;
                printf("Descendo via %s\n. FOB: %.3lf", flight_ref->current_procedure->id.c_str(), flight_ref->fob);

                flight_ref->flight_phase = DESCENDING;
            }
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

void flight::disconnect_radio(int signal) {
    pthread_exit(0);
}

void* flight::run(void* thread_target) {
    flight* flight_obj = (flight *) thread_target;
    time_t timer; // Current time
    signal(SIGINT, disconnect_radio);

    const char* callsign = flight_obj->callsign.c_str();
    char message_text[MESSAGE_SIZE];
    std::string message_buff;

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
        
        snprintf(message_text, MESSAGE_SIZE, "%s no solo, solicitando autorização de rota para %s\n", callsign, flight_obj->destination.c_str());
        message_buff = message_text;

        frequencies[flight_obj->current_radio_frequency].transmit(
            callsign, 
            flight_obj->origin.c_str(),
            message_buff.c_str(),
            (void *) &flight_obj->flight_phase,
            CHECK_IN
        );
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

        snprintf(message_text, MESSAGE_SIZE, "%s com você no FL %.0ld, chegada para %s\n", callsign, flight_obj->airplane.current_alt / 100, flight_obj->destination.c_str());
        message_buff = message_text;

        frequencies[flight_obj->current_radio_frequency].transmit(
            callsign, 
            frequencies[flight_obj->current_radio_frequency].callsign.c_str(),
            message_buff.c_str(),
            (void *) &flight_obj->flight_phase,
            CHECK_IN
        );
    }
    
    // Flight execution
    while(true) {
        time(&timer);
        radio_message msg = frequencies[flight_obj->current_radio_frequency].listen(callsign);
        if (!msg.blank) {
            evaluate_message(flight_obj, msg);
        }
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
