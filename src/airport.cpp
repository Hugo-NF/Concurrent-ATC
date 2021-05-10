#include "../include/airport.h"

void* airport::run(void* thread_target) {
    airport* airport_obj = (airport *) thread_target;
    printf("%s_TWR is now online on %.3lf MHz\n", airport_obj->icao_id.c_str(), airport_obj->radio_frequency);
    
    
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
                frequencies[this->radio_frequency] = &this->radio_channel;
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