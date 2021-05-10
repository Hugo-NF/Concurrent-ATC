#include "../include/tma.h"

void tma::evaluate_message(tma* tma_ref, radio_message msg) {
    char message_text[MESSAGE_SIZE];
    
    printf("[%s] %s", msg.recipient.c_str(), msg.content.c_str());
    
    switch (msg.type) {
        case CHECK_IN: {
            int * flight_phase = (int *) msg.arg;
            
            // Arriving
            if (* flight_phase == CRUISING) {
                tma_ref->flights_on_service[std::string(msg.sender)] = *flight_phase;
                std::string dest_id = tma_ref->flights_on_terminal[std::string(msg.sender)].destination;
                airport* dest_airport = &tma_ref->airports[dest_id];

                snprintf(
                    message_text,
                    MESSAGE_SIZE,
                    "%s contato radar estabelecido, aguarde para sequenciamento em %s\n",
                    msg.sender.c_str(),
                    dest_id.c_str()
                );

                frequencies[tma_ref->radio_frequency].transmit(
                    msg.recipient.c_str(),
                    msg.sender.c_str(),
                    message_text,
                    (void *) dest_airport,
                    CHECK_IN
                );
            }
            break;
        }
        case CHECK_OUT: {
            break;
        }
        case DESCEND_REQUEST: {
            int current_phase = tma_ref->flights_on_service[msg.sender];
            if(current_phase == CRUISING) {
                tma_ref->flights_on_service[std::string(msg.sender)] = DESCENDING;

                std::string dest_id = tma_ref->flights_on_terminal[std::string(msg.sender)].destination;
                airport* dest_airport = &tma_ref->airports[dest_id];
                std::string landing_runway = dest_airport->active_runways[rand() % dest_airport->active_runways.size()]; 
                std::string landing_star = dest_airport->runways[landing_runway].active_stars[rand() % dest_airport->runways[landing_runway].active_stars.size()];

                snprintf(
                    message_text,
                    MESSAGE_SIZE,
                    "%s autorizado descida via %s, esperada pista %s em %s\n",
                    msg.sender.c_str(),
                    landing_star.c_str(),
                    landing_runway.c_str(),
                    dest_id.c_str()
                );

                frequencies[tma_ref->radio_frequency].transmit(
                    msg.recipient.c_str(),
                    msg.sender.c_str(),
                    message_text,
                    (void *) &dest_airport->runways[landing_runway].stars[landing_star],
                    DESCEND_CLEARANCE
                );
            }

            break;
        }
        case CLIMBING_REQUEST: {
            break;
        }
        case AFTER_TAKEOFF: {
            break;
        }
        case AFTER_GO_AROUND: {
            break;
        }
        case ENTER_HOLDING_REQUEST: {
            break;
        }
        case EXIT_HOLDING_REQUEST: {
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

void* tma::run(void* thread_target) {
    tma* tma_obj = (tma *) thread_target;
    printf("%s is now online on %.3lf MHz.\n", tma_obj->id.c_str(), tma_obj->radio_frequency);

    time_t timer; // Current time

    // Starting airports
    std::list<pthread_t> airport_threads;
    for(auto it = tma_obj->airports.begin(); it != tma_obj->airports.end(); ++it) {
        airport_threads.emplace_back();
        pthread_create(&airport_threads.back(), NULL, airport::run, (void *) &it.operator->()->second);
    }

    // Starting flights
    std::list<pthread_t> flight_threads;
    for(auto it = tma_obj->flights_on_terminal.begin(); it != tma_obj->flights_on_terminal.end(); ++it) {
        flight_threads.emplace_back();
        pthread_create(&flight_threads.back(), NULL, flight::run, (void *) &it.operator->()->second);
    }

    // TMA execution
    while(true) {
        time(&timer);
        radio_message msg = frequencies[tma_obj->radio_frequency].listen(tma_obj->id.c_str());
        if (!msg.blank) {
            evaluate_message(tma_obj, msg);
        }

        if (tma_obj->flights_on_terminal.size() == 0) {
            // Waiting airport threads to finish
            for(auto it = airport_threads.begin(); it != airport_threads.end(); ++it) {
                pthread_kill(it.operator*(), SIGINT);
            }
            break;
        }
    }

    // Waiting airport threads to finish
    for(auto it = airport_threads.begin(); it != airport_threads.end(); ++it) {
        pthread_join(it.operator*(), 0);
    }
    printf("All airports shutdown operations...\n");

    // Waiting flights threads to finish
    for(auto it = flight_threads.begin(); it != flight_threads.end(); ++it) {
        pthread_join(it.operator*(), 0);
    }
    printf("All airplanes disconnected from terminal...\n");

    pthread_exit(0);
}

int tma::load_from_json(const char* filename){
    
    FILE *fp;
    struct stat filestatus;
    int file_size;
    char* file_contents;
    json_char* json;
    json_value* value;

    if (stat(filename, &filestatus) != 0) {
        printf("File error: File %s not found\n", filename);
        return 1;
    }

    file_size = filestatus.st_size;
    file_contents = (char*)malloc(filestatus.st_size);

    if (file_contents == NULL) {
        printf("Memory error: unable to allocate %d bytes\n", file_size);
        return 2;
    }

    fp = fopen(filename, "rt");
    if (fp == nullptr) {
        printf("Unable to open file: %s\n", filename);
        fclose(fp);
        free(file_contents);
        return 3;
    }

    if (fread(file_contents, file_size, 1, fp) != 1) {
        printf("Unable to read content of %s\n", filename);
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
                this->id = value->u.object.values[x].value->u.string.ptr;
                break;
            }
            case 1: {
                this->description = value->u.object.values[x].value->u.string.ptr;
                break;
            }
            case 2: {
                this->radio_frequency = value->u.object.values[x].value->u.dbl;
                frequencies[this->radio_frequency] = radio();
                frequencies[this->radio_frequency].callsign = this->id;
                break;
            }
            case 3: {
                auto array = value->u.object.values[x].value->u.array;
                for (unsigned int x = 0; x < array.length; x++) {
                    std::string airport_name = array.values[x]->u.string.ptr;
                    airport new_airport = airport();
                    new_airport.load_from_json(airport_name.c_str());
                    this->airports[airport_name] = new_airport;
                }
                break;
            }
            default: {
                printf("Warning: Unexpected property %s in %s\n", value->u.object.values[x].name, filename);
                break;
            }
        }
    }

    json_value_free(value);
    free(file_contents);
    return 0;
}

int tma::load_flights(const char* filename){

    FILE *fp;
    struct stat filestatus;
    int file_size;
    char* file_contents;
    json_char* json;
    json_value* value;

    if (stat(filename, &filestatus) != 0) {
        printf("File error: File %s not found\n", filename);
        return 1;
    }

    file_size = filestatus.st_size;
    file_contents = (char*)malloc(filestatus.st_size);

    if (file_contents == NULL) {
        printf("Memory error: unable to allocate %d bytes\n", file_size);
        return 2;
    }

    fp = fopen(filename, "rt");
    if (fp == nullptr) {
        printf("Unable to open file: %s\n", filename);
        fclose(fp);
        free(file_contents);
        return 3;
    }

    if (fread(file_contents, file_size, 1, fp) != 1) {
        printf("Unable to read content of %s\n", filename);
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
                auto array = value->u.object.values[x].value->u.array;
                for (unsigned int arr_index = 0; arr_index < array.length; arr_index++) {
                    json_value* flight_obj = array.values[arr_index];
                    flight new_flight = flight();
                    new_flight.load_from_json_value(flight_obj);

                    // Set initial values
                    // Departing aircraft
                    if (new_flight.distance_to_tod == 0 && new_flight.time_to_pushback > 0) {
                        new_flight.flight_phase = ON_GROUND;
                        
                        new_flight.target_airport = &this->airports[new_flight.origin];
                        new_flight.current_radio_frequency = new_flight.target_airport->radio_frequency;
                       
                        new_flight.airplane.current_speed = 0;
                        new_flight.airplane.current_alt = new_flight.target_airport->elevation_ft;
                        new_flight.airplane.current_ff = 0;
                    }
                    // Arriving aircraft
                    else {
                        new_flight.flight_phase = CRUISING;

                        new_flight.current_radio_frequency = this->radio_frequency;
                        new_flight.target_airport = &this->airports[new_flight.destination];
 
                        new_flight.airplane.current_speed = new_flight.airplane.cruise_spd;
                        new_flight.airplane.current_alt = 36000; 
                        // Constant for now. Not added to JSON because we always presume that the aircraft is able to respect the Vertical Profile,
                        // since we're not emulating any malfunctions, weather and so on.
                        new_flight.airplane.current_ff = new_flight.airplane.cruise_ff;
                    }

                    flights_on_terminal[new_flight.callsign] = new_flight;
                }
                break;
            }
            default: {
                printf("Warning: Unexpected property %s in %s\n", value->u.object.values[x].name, filename);
                break;
            }
        }
    }

    json_value_free(value);
    free(file_contents);
    return 0;
}

void tma::print_info(){
    printf("Terminal ID: %s\n", this->id.c_str());
    printf("Description: %s\n", this->description.c_str());
    printf("Radio frequency: %.3lf MHz\n", this->radio_frequency);
    
    printf("Airports:\n");
    for(auto it = airports.begin(); it != airports.end(); ++it) {
        auto airport = it.operator*();
        printf("\t%s:\n", airport.first.c_str());
        airport.second.print_info();
    }
}
