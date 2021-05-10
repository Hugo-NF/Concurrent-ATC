#include "../include/tma.h"

void* tma::run(void* thread_target) {
    tma* tma_obj = (tma *) thread_target;
    printf("%s is now online on %.3lf MHz.\n", tma_obj->id.c_str(), tma_obj->radio_frequency);

    // Starting airports
    std::list<pthread_t> flight_threads;
    for(auto it = tma_obj->airports.begin(); it != tma_obj->airports.end(); ++it) {
        flight_threads.emplace_back();
        pthread_create(&flight_threads.back(), NULL, airport::run, (void *) &it.operator->()->second);
    }

    // Waiting airport threads to finish
    for(auto it = flight_threads.begin(); it != flight_threads.end(); ++it) {
        pthread_join(it.operator*(), 0);
    }

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
                frequencies[this->radio_frequency] = &this->radio_channel;
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
                    flights_on_terminal.push_back(new_flight);
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
