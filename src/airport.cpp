#include "../include/airport.h"

void airport::evaluate_message(airport* airport_ref, radio_message msg) {
    char message_text[MESSAGE_SIZE];

    printf("[%s_TWR] %s", msg.recipient.c_str(), msg.content.c_str());
    
    switch (msg.type) {
        case CHECK_IN: {
            int * flight_phase = (int *) msg.arg;
            switch (*flight_phase) {
                case APPROACH: {
                    airport_ref->flights_on_service[msg.sender] = APPROACH;
                    std::string landing_runway = airport_ref->active_runways[rand() % airport_ref->active_runways.size()];

                    snprintf(
                        message_text,
                        MESSAGE_SIZE,
                        "%s, bom dia. Prossiga na aproximação, aguarde autorização para pouso na %s\n",
                        msg.sender.c_str(),
                        landing_runway.c_str()
                    );

                    frequencies[airport_ref->radio_frequency].transmit(
                        msg.recipient.c_str(),
                        msg.sender.c_str(),
                        message_text,
                        (void *) &airport_ref->runways[landing_runway],
                        CHECK_IN
                    );
                }
            
                default:
                    break;
            }
            break;
        }
        case CHECK_OUT: {
            break;
        }
        case TAXI_REQUEST: {
            break;
        }
        case TAKEOFF_REQUEST: {
            break;
        }
        case LANDING_REQUEST: {
            bool landing_autorized = false;
            for(unsigned int rwy_idx = 0; rwy_idx < airport_ref->active_runways.size(); rwy_idx++) {
                if (airport_ref->runways[airport_ref->active_runways[rwy_idx]].try_join_runway(msg.sender.c_str())) {
                    // Autorize landing
                    landing_autorized = true;
                    airport_ref->flights_on_service[msg.sender] = LANDING;
                    snprintf(
                        message_text,
                        MESSAGE_SIZE,
                        "%s, pouso autorizado, pista %s. Vento 102 com 7 nós. QNH 1021\n",
                        msg.sender.c_str(),
                        airport_ref->runways[airport_ref->active_runways[rwy_idx]].id.c_str()
                    );

                    frequencies[airport_ref->radio_frequency].transmit(
                        msg.recipient.c_str(),
                        msg.sender.c_str(),
                        message_text,
                        (void *) &airport_ref->runways[airport_ref->active_runways[rwy_idx]],
                        LANDING_CLEARANCE
                    );
                    break;
                }
            }
            if(!landing_autorized) {
                airport_ref->flights_on_service[msg.sender] = GO_AROUND;
                snprintf(
                    message_text,
                    MESSAGE_SIZE,
                    "%s, arremeta.\n",
                    msg.sender.c_str()
                );

                frequencies[airport_ref->radio_frequency].transmit(
                    msg.recipient.c_str(),
                    msg.sender.c_str(),
                    message_text,
                    NULL,
                    GO_AROUND_REQUEST
                );
            }

            break;
        }
        case AFTER_LANDING: {
            runway* vacating_runway = (runway *) msg.arg;
            vacating_runway->leave_runway(); 
            break;
        }
        case MAYDAY_CALL: {
            break;
        }
        case PANPAN_CALL: {
            break;
        }
        default: {
            break;
        }
    }
}

void airport::finish_operation(int signal) {
    printf("Signal %d received\n", signal);
    pthread_exit(0);
}

void* airport::run(void* thread_target) {
    airport* airport_obj = (airport *) thread_target;
    time_t timer; // Current time
    const char* callsign = airport_obj->icao_id.c_str();

    printf("%s_TWR is now online on %.3lf MHz\n", airport_obj->icao_id.c_str(), airport_obj->radio_frequency);
    
    signal(SIGINT, finish_operation);
    
    while(1) {
        time(&timer);
        radio_message msg = frequencies[airport_obj->radio_frequency].listen(callsign);
        if (!msg.blank) {
            airport::evaluate_message(airport_obj, msg);
        }
        sleep(rand() % 15);
    }

    pthread_exit(0);
}

int airport::load_from_json(const char* airport_icao) {
    std::string filename = "data/airports/" + std::string(airport_icao) + ".json";    
    
    FILE *fp;
    struct stat filestatus;
    int file_size;
    char* file_contents;
    json_char* json;
    json_value* value;

    if (stat(filename.c_str(), &filestatus) != 0) {
        printf("File error: File %s not found\n", filename.c_str());
        return 1;
    }

    file_size = filestatus.st_size;
    file_contents = (char*)malloc(filestatus.st_size);

    if (file_contents == NULL) {
        printf("Memory error: unable to allocate %d bytes\n", file_size);
        return 2;
    }

    fp = fopen(filename.c_str(), "rt");
    if (fp == nullptr) {
        printf("Unable to open file: %s\n", filename.c_str());
        fclose(fp);
        free(file_contents);
        return 3;
    }

    if (fread(file_contents, file_size, 1, fp) != 1) {
        printf("Unable to read content of %s\n", filename.c_str());
        fclose(fp);
        free(file_contents);
        return 4;
    }
    fclose(fp);

    json = (json_char*)file_contents;

    value = json_parse(json,file_size);

    if (value == nullptr) {
        printf("Unable to parse JSON data\n");
        free(file_contents);
        return 5;
    }

    int obj_length = value->u.object.length;
    for (int x = 0; x < obj_length; x++) {
        switch (x) {
            case 0: {
                this->icao_id = value->u.object.values[x].value->u.string.ptr;
                break;
            }
            case 1: {
                this->iata_id = value->u.object.values[x].value->u.string.ptr;
                break;
            }
            case 2: {
                this->name = value->u.object.values[x].value->u.string.ptr;
                break;
            }
            case 3: {
                this->elevation_ft = value->u.object.values[x].value->u.integer;
                break;
            }
            case 4: {
                this->radio_frequency = value->u.object.values[x].value->u.dbl;
                frequencies[this->radio_frequency] = radio();
                frequencies[this->radio_frequency].callsign = this->icao_id;
                break;
            }
            case 5: {
                auto array = value->u.object.values[x].value->u.array;
                for (unsigned int x = 0; x < array.length; x++) {
                    auto current_runway = array.values[x];
                    runway new_runway = runway();
                    new_runway.load_from_json_value(this->icao_id.c_str(), current_runway);
                    this->runways[new_runway.id] = new_runway;
                }
                break;
            }
            case 6: {
                auto actives_object = value->u.object.values[x].value->u.object;
                int actives_length = actives_object.length;
                for (int x = 0; x < actives_length; x++) {
                    switch (x) {
                        case 0: {
                            auto array = actives_object.values[x].value->u.array;
                            for (unsigned int y = 0; y < array.length; y++) {
                                auto current_active = array.values[y]->u.string.ptr;
                                this->active_runways.push_back(current_active); 
                            }
                            break;
                        }
                        case 1: {
                            for(unsigned int i = 0; i < this->active_runways.size(); i++) {
                                auto array = actives_object.values[x].value->u.array;
                                for (unsigned int y = 0; y < array.length; y++) {
                                    auto current_active = array.values[y]->u.string.ptr;
                                    this->runways[this->active_runways[i]].active_sids.push_back(current_active); 
                                }
                            }
                            break;
                        }
                        case 2: {
                            for(unsigned int i = 0; i < this->active_runways.size(); i++) {
                                auto array = actives_object.values[x].value->u.array;
                                for (unsigned int y = 0; y < array.length; y++) {
                                    auto current_active = array.values[y]->u.string.ptr;
                                    this->runways[this->active_runways[i]].active_stars.push_back(current_active); 
                                }
                            }
                            break;
                        }
                        case 3: {
                            for(unsigned int i = 0; i < this->active_runways.size(); i++) {
                                auto array = actives_object.values[x].value->u.array;
                                for (unsigned int y = 0; y < array.length; y++) {
                                    auto current_active = array.values[y]->u.string.ptr;
                                    this->runways[this->active_runways[i]].active_approaches.push_back(current_active); 
                                }
                            }
                            break;
                        }
                        default: {
                            printf("Warning: Unexpected active entity %s on airport %s\n", actives_object.values[x].name, this->icao_id.c_str());
                            break;
                        }
                    }
                }
                break;
            }
            default: {
                printf("Warning: Unexpected property %s in %s\n", value->u.object.values[x].name, filename.c_str());
                break;
            }
        }
    }

    json_value_free(value);
    free(file_contents);

    return 0;
}

void airport::print_info() {
    printf("\t\tICAO: %s\n", this->icao_id.c_str());
    printf("\t\tIATA: %s\n", this->iata_id.c_str());
    printf("\t\tName: %s\n", this->name.c_str());
    printf("\t\tElevation (ft): %ld\n", this->elevation_ft);
    printf("\t\tRadio frequency: %.3lf MHz\n", this->radio_frequency);
    printf("\t\tRunways:\n");

    for(auto it = runways.begin(); it != runways.end(); ++it) {
        it.operator*().second.print_info();
        printf("\n");
    }    
}