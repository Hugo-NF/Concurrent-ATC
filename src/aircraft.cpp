#include "../include/aircraft.h"

int aircraft::load_from_json(std::string aircraft_type) {
    std::string filename = "data/aircrafts/" + aircraft_type + ".json";

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
                this->descent_spd = value->u.object.values[x].value->u.integer;
                break;
            }
            case 1: {
                this->approach_spd = value->u.object.values[x].value->u.integer;
                break;
            }
            case 2: {
                this->landing_spd = value->u.object.values[x].value->u.integer;
                break;
            }
            case 3: {
                this->takeoff_spd = value->u.object.values[x].value->u.integer;
                break;
            }
            case 4: {
                this->climb_spd = value->u.object.values[x].value->u.integer;
                break;
            }
            case 5: {
                this->cruise_spd = value->u.object.values[x].value->u.integer;
                break;
            }
            case 6: {
                this->takeoff_distance = value->u.object.values[x].value->u.integer;
                break;
            }
            case 7: {
                this->landing_distance = value->u.object.values[x].value->u.integer;
                break;
            }
            case 8: {
                this->descent_ff = value->u.object.values[x].value->u.dbl;
                break;
            }
            case 9: {
                this->approach_ff = value->u.object.values[x].value->u.dbl;
                break;
            }
            case 10: {
                this->takeoff_ff = value->u.object.values[x].value->u.dbl;
                break;
            }
            case 11: {
                this->climb_ff = value->u.object.values[x].value->u.dbl;
                break;
            }
            case 12: {
                this->cruise_ff = value->u.object.values[x].value->u.dbl;
                this->landing_ff = this->cruise_ff;
                break;
            }
            case 13: {
                this->idle_ff = value->u.object.values[x].value->u.dbl;
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

double aircraft::calculate_remaining_fuel(double fob, double time) {
    double result = (time * this->current_ff) / 3600;
    return fob - result;
}

unsigned int aircraft::calculate_next_waypoint(double distance) {
    double result = (distance * 3600) / this->current_speed;
    return (unsigned int) floor(result * 0.1); // Make execution faster (times decrease 90%). TODO: Remove this coefficent
}