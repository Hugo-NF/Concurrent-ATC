#include "../include/flight.h"

void flight::evaluate_message(flight* flight_ref, radio_message msg) {
    std::size_t found = msg.sender.find_first_of("_");
    if(found != std::string::npos) {
        printf("[%s] %s", msg.sender.c_str(), msg.content.c_str());
    } 
    else {
        printf("[%s_TWR] %s", msg.sender.c_str(), msg.content.c_str());
    }
    
    char message_text[MESSAGE_SIZE];

    switch (msg.type) {
        case CHECK_IN: {
            switch (flight_ref->flight_phase) {
                case CRUISING: {
                    flight_ref->target_airport = (airport *) msg.arg;

                    snprintf(
                        message_text,
                        MESSAGE_SIZE,
                        "%s para %s, ciente. %.2lf milhas para o ideal de descida\n",
                        msg.recipient.c_str(),
                        msg.sender.c_str(),
                        flight_ref->distance_to_tod
                    );

                    frequencies[flight_ref->current_radio_frequency].transmit(
                        flight_ref->callsign.c_str(), 
                        frequencies[flight_ref->current_radio_frequency].callsign.c_str(),
                        message_text,
                        NULL,
                        DESCEND_REQUEST
                    );
                    unsigned int distance_time = flight_ref->airplane.calculate_next_waypoint(flight_ref->distance_to_tod);
                    flight_ref->fob = flight_ref->airplane.calculate_remaining_fuel(flight_ref->fob, distance_time);
                    sleep(distance_time);
                    break;  
                }
                case APPROACH: {
                    if(flight_ref->current_approach != NULL) { // IFR flights
                        for(unsigned int waypoint_idx = 0; waypoint_idx < flight_ref->current_approach->approach_fixes.size(); waypoint_idx++) {
                            waypoint current_pos = flight_ref->current_approach->approach_fixes[waypoint_idx];
                            if (current_pos.alt_restriction > 0)
                                flight_ref->airplane.current_alt = current_pos.alt_restriction;
                            if (current_pos.spd_restriction > 0 && current_pos.spd_restriction < flight_ref->airplane.current_speed)
                                flight_ref->airplane.current_speed = current_pos.spd_restriction;
                            if (current_pos.heading > -1) {
                                flight_ref->airplane.current_speed = flight_ref->airplane.landing_spd;
                                flight_ref->airplane.current_ff = flight_ref->airplane.landing_ff;
                            }
                            printf("Tracking >> %s cruzando %s. %ld waypoints restantes. FF: %.3lf tons/h. FOB: %.3lf tons. Alt: %ld ft. IAS: %ld kts.\n", 
                                flight_ref->callsign.c_str(),
                                current_pos.id.c_str(),
                                ((flight_ref->current_approach->approach_fixes.size() - 1) - waypoint_idx),
                                flight_ref->airplane.current_ff,
                                flight_ref->fob,
                                flight_ref->airplane.current_alt,
                                flight_ref->airplane.current_speed
                            );

                            if (waypoint_idx == flight_ref->current_approach->approach_fixes.size() - 1) {
                                snprintf(
                                    message_text,
                                    MESSAGE_SIZE,
                                    "%s para %s_TWR. Curta final %s. Solicitando permiss??o para pouso\n",
                                    msg.recipient.c_str(),
                                    msg.sender.c_str(),
                                    flight_ref->current_approach->id.c_str()
                                );

                                frequencies[flight_ref->current_radio_frequency].transmit(
                                    flight_ref->callsign.c_str(), 
                                    frequencies[flight_ref->current_radio_frequency].callsign.c_str(),
                                    message_text,
                                    NULL,
                                    LANDING_REQUEST
                                );
                            }
                            else {
                                unsigned int distance_time = flight_ref->airplane.calculate_next_waypoint(current_pos.distance_to_next);
                                flight_ref->fob = flight_ref->airplane.calculate_remaining_fuel(flight_ref->fob, distance_time);
                                sleep(distance_time);
                            }
                        }
                    }
                    else {
                        snprintf(
                            message_text,
                            MESSAGE_SIZE,
                            "%s aproximando da MDA. Solicitando autoriza????o para pouso\n",
                            msg.recipient.c_str()
                        );

                        frequencies[flight_ref->current_radio_frequency].transmit(
                            flight_ref->callsign.c_str(),
                            frequencies[flight_ref->current_radio_frequency].callsign.c_str(),
                            message_text,
                            NULL,
                            LANDING_REQUEST
                        );

                        flight_ref->airplane.current_speed = flight_ref->airplane.landing_spd;
                        flight_ref->airplane.current_ff = flight_ref->airplane.landing_ff;
                        unsigned int distance_time = flight_ref->airplane.calculate_next_waypoint(rand() % 10);
                        flight_ref->fob = flight_ref->airplane.calculate_remaining_fuel(flight_ref->fob, distance_time);
                        sleep(distance_time);
                    }
                    break;
                } 
            }
            break;
        }
        case CHECK_OUT: {
            switch (flight_ref->flight_phase) {
                case DESCENDING: {
                    flight_ref->current_approach = (approach *) msg.arg;
                    snprintf(
                        message_text,
                        MESSAGE_SIZE,
                        "%s transferindo para torre. Obrigado pelo excelente ATC!\n",
                        msg.recipient.c_str()
                    );

                    frequencies[flight_ref->current_radio_frequency].transmit(
                        flight_ref->callsign.c_str(), 
                        frequencies[flight_ref->current_radio_frequency].callsign.c_str(),
                        message_text,
                        NULL,
                        OTHER
                    );

                    // Switch to new controller
                    flight_ref->current_radio_frequency = flight_ref->target_airport->radio_frequency;

                    snprintf(
                        message_text,
                        MESSAGE_SIZE,
                        "Bom dia %s_TWR, aqui ?? o %s. Iniciando aproxima????o %s.\n",
                        flight_ref->target_airport->icao_id.c_str(),
                        flight_ref->callsign.c_str(),
                        flight_ref->current_approach->id.c_str()
                    );

                    frequencies[flight_ref->current_radio_frequency].transmit(
                        flight_ref->callsign.c_str(), 
                        frequencies[flight_ref->current_radio_frequency].callsign.c_str(),
                        message_text,
                        (void *) &flight_ref->flight_phase,
                        CHECK_IN
                    );

                    flight_ref->flight_phase = APPROACH;
                    flight_ref->airplane.current_ff = flight_ref->airplane.approach_ff;
                    flight_ref->airplane.current_speed = flight_ref->airplane.approach_spd;
                    break;
                }
                case DESCENDING_VFR: {
                    snprintf(
                        message_text,
                        MESSAGE_SIZE,
                        "%s transferindo para torre. Obrigado pelo excelente ATC!\n",
                        msg.recipient.c_str()
                    );

                    frequencies[flight_ref->current_radio_frequency].transmit(
                        flight_ref->callsign.c_str(), 
                        frequencies[flight_ref->current_radio_frequency].callsign.c_str(),
                        message_text,
                        NULL,
                        OTHER
                    );

                    // Switch to new controller
                    flight_ref->current_radio_frequency = flight_ref->target_airport->radio_frequency;

                    snprintf(
                        message_text,
                        MESSAGE_SIZE,
                        "Bom dia %s_TWR, aqui ?? o %s. Iniciando aproxima????o visual, pista %s.\n",
                        flight_ref->target_airport->icao_id.c_str(),
                        flight_ref->callsign.c_str(),
                        flight_ref->current_runway->id.c_str()
                    );

                    frequencies[flight_ref->current_radio_frequency].transmit(
                        flight_ref->callsign.c_str(), 
                        frequencies[flight_ref->current_radio_frequency].callsign.c_str(),
                        message_text,
                        (void *) &flight_ref->flight_phase,
                        CHECK_IN
                    );
                    
                    flight_ref->flight_phase = APPROACH;
                    flight_ref->airplane.current_ff = flight_ref->airplane.approach_ff;
                    flight_ref->airplane.current_speed = flight_ref->airplane.approach_spd;
                    break;
                }
                case CLIMBING: {
                    break;
                }
                case CLIMBING_VFR: {
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case TAXI_CLEARANCE: {
            break;
        }
        case DESCEND_CLEARANCE: {
            if(flight_ref->flight_phase == CRUISING) {
                flight_ref->current_procedure = (sid_star *) msg.arg;
                flight_ref->flight_phase = DESCENDING;

                flight_ref->airplane.current_speed = flight_ref->airplane.descent_spd;
                flight_ref->airplane.current_ff = flight_ref->airplane.descent_ff;

                for(unsigned int waypoint_idx = 0; waypoint_idx < flight_ref->current_procedure->fixes.size(); waypoint_idx++) {
                    waypoint current_pos = flight_ref->current_procedure->fixes[waypoint_idx];
                    if (current_pos.alt_restriction > 0)
                        flight_ref->airplane.current_alt = current_pos.alt_restriction;
                    if (current_pos.spd_restriction > 0 && current_pos.spd_restriction < flight_ref->airplane.current_speed)
                        flight_ref->airplane.current_speed = current_pos.spd_restriction;
                    
                    printf("Tracking >> %s cruzando %s. %ld waypoints restantes. FF: %.3lf tons/h. FOB: %.3lf tons. Alt: %ld ft. IAS: %ld kts.\n", 
                        flight_ref->callsign.c_str(),
                        current_pos.id.c_str(),
                        ((flight_ref->current_procedure->fixes.size() - 1) - waypoint_idx),
                        flight_ref->airplane.current_ff,
                        flight_ref->fob,
                        flight_ref->airplane.current_alt,
                        flight_ref->airplane.current_speed
                    );

                    if (current_pos.distance_to_next == 0) {
                        snprintf(
                            message_text,
                            MESSAGE_SIZE,
                            "%s para %s. Iniciando procedimento de aproxima????o\n",
                            msg.recipient.c_str(),
                            msg.sender.c_str()
                        );

                        frequencies[flight_ref->current_radio_frequency].transmit(
                            flight_ref->callsign.c_str(), 
                            frequencies[flight_ref->current_radio_frequency].callsign.c_str(),
                            message_text,
                            NULL,
                            CHECK_OUT
                        );
                    } 
                    else {
                        unsigned int distance_time = flight_ref->airplane.calculate_next_waypoint(current_pos.distance_to_next);
                        flight_ref->fob = flight_ref->airplane.calculate_remaining_fuel(flight_ref->fob, distance_time);
                        sleep(distance_time);
                    }
                }
                
            }
            break;
        }
        case DESCEND_VFR_CLEARANCE: {
            if(flight_ref->flight_phase == CRUISING) {
                flight_ref->current_runway = (runway *) msg.arg;
                flight_ref->flight_phase = DESCENDING_VFR;

                flight_ref->airplane.current_speed = flight_ref->airplane.descent_spd;
                flight_ref->airplane.current_ff = flight_ref->airplane.descent_ff;

                unsigned int distance_time = flight_ref->airplane.calculate_next_waypoint(rand() % 40);
                flight_ref->fob = flight_ref->airplane.calculate_remaining_fuel(flight_ref->fob, distance_time);
                sleep(distance_time);

                snprintf(
                    message_text,
                    MESSAGE_SIZE,
                    "%s para %s. Ingressando final para pista %s de %s\n",
                    msg.recipient.c_str(),
                    msg.sender.c_str(),
                    flight_ref->current_runway->id.c_str(),
                    flight_ref->destination.c_str()
                );

                frequencies[flight_ref->current_radio_frequency].transmit(
                    flight_ref->callsign.c_str(), 
                    frequencies[flight_ref->current_radio_frequency].callsign.c_str(),
                    message_text,
                    NULL,
                    CHECK_OUT
                );
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
            flight_ref->current_runway = (runway *) msg.arg;
            double distance_to_runway;
            if(flight_ref->current_approach == NULL) {
                distance_to_runway = rand() % 5;
            }
            else {
                distance_to_runway = flight_ref->current_approach->approach_fixes[flight_ref->current_approach->approach_fixes.size() - 1].distance_to_next;
            }
            double landing_distance = flight_ref->airplane.landing_distance / 6076; //Convert ft to nautical miles (nm)

            printf("[%s_TWR] Pouso autorizado, pista %s. QNH 1021. %s\n", msg.sender.c_str(), flight_ref->current_runway->id.c_str(), msg.recipient.c_str());

            unsigned int distance_time = flight_ref->airplane.calculate_next_waypoint(distance_to_runway + landing_distance);
            flight_ref->fob = flight_ref->airplane.calculate_remaining_fuel(flight_ref->fob, distance_time);
            printf("Event >> %s est?? a %d segundos para o pouso.\n", flight_ref->callsign.c_str(), distance_time);
            sleep(distance_time);

            printf("Event >> %s fez aquele pouso manteiga.\n", flight_ref->callsign.c_str());
            snprintf(
                message_text,
                MESSAGE_SIZE,
                "%s. Em solo, livrando %s na taxiway A\n",
                msg.recipient.c_str(),
                flight_ref->current_runway->id.c_str()
            );


            frequencies[flight_ref->current_radio_frequency].transmit(
                flight_ref->callsign.c_str(), 
                frequencies[flight_ref->current_radio_frequency].callsign.c_str(),
                message_text,
                (void *) flight_ref->current_runway,
                AFTER_LANDING
            );
            break;
        }
        case GO_AROUND_REQUEST: {
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
        
        snprintf(message_text, MESSAGE_SIZE, "%s no solo, solicitando autoriza????o de rota para %s\n", callsign, flight_obj->destination.c_str());
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

        snprintf(message_text, MESSAGE_SIZE, "%s com voc?? no FL %.0ld, chegada para %s\n", callsign, flight_obj->airplane.current_alt / 100, flight_obj->destination.c_str());
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
