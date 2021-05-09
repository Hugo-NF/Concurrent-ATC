#include "../include/waypoint.h"

int sid_star::load_from_json(std::string proc_path, std::string procedure_id) {
    this->id = procedure_id;

    std::string filename = "data/" + proc_path + procedure_id + ".json";    
    
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
    for (int obj_index = 0; obj_index < obj_length; obj_index++) {
        switch (obj_index) {
            case 0: {
                break;
            }
            case 1: {
                break;
            }
            case 2: {
                auto array = value->u.object.values[obj_index].value->u.array;
                for (unsigned int arr_index = 0; arr_index < array.length; arr_index++) {
                    auto current_waypoint = array.values[arr_index];
                    
                    std::string id;
                    long alt_restriction;
                    long spd_restriction;
                    long distance_to_next;

                    for(unsigned int obj_index = 0; obj_index < current_waypoint->u.object.length; obj_index++) {
                        auto current_value = current_waypoint->u.object.values[obj_index].value;
                        switch (obj_index) {
                        case 0:
                            id = current_value->u.string.ptr;
                            break;
                        case 1:
                            alt_restriction = current_value->u.integer;
                            break;
                        case 2:
                            spd_restriction = current_value->u.integer;
                            break;
                        case 3:
                            distance_to_next = current_value->u.dbl;
                            break;
                        default:
                            printf("Warning: Unexpected property %s in %s.json\n", current_waypoint->u.object.values[obj_index].name, procedure_id.c_str());
                            break;
                        }
                    }

                    this->fixes.emplace_back(id, alt_restriction, spd_restriction, distance_to_next);
                }
                break;
            }
            default: {
                printf("Warning: Unexpected property %s in %s\n", value->u.object.values[obj_index].name, filename.c_str());
                break;
            }
        }
    }

    json_value_free(value);
    free(file_contents);

    return 0;
}


int approach::load_from_json(const char* airport_icao, std::string procedure_id, std::string runway_id) {
    this->id = procedure_id;

    std::string filename = "data/approaches/" + std::string(airport_icao) + "-" + procedure_id + "-" + runway_id + ".json";    
    
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
    for (int obj_index = 0; obj_index < obj_length; obj_index++) {
        switch (obj_index) {
            case 0: {
                break;
            }
            case 1: {
                break;
            }
            case 2: {
                auto array = value->u.object.values[obj_index].value->u.array;
                for (unsigned int arr_index = 0; arr_index < array.length; arr_index++) {
                    auto current_waypoint = array.values[arr_index];
                    
                    std::string id;
                    long alt_restriction;
                    long spd_restriction;
                    long heading;
                    double distance_to_next;

                    for(unsigned int obj_index = 0; obj_index < current_waypoint->u.object.length; obj_index++) {
                        auto current_value = current_waypoint->u.object.values[obj_index].value;
                        switch (obj_index) {
                        case 0:
                            id = current_value->u.string.ptr;
                            break;
                        case 1:
                            alt_restriction = current_value->u.integer;
                            break;
                        case 2:
                            spd_restriction = current_value->u.integer;
                            break;
                        case 3:
                            heading = current_value->u.integer;
                            break;
                        case 4:
                            distance_to_next = current_value->u.dbl;
                            break;
                        default:
                            printf("Warning: Unexpected property %s in %s.json\n", current_waypoint->u.object.values[obj_index].name, procedure_id.c_str());
                            break;
                        }
                    }

                    this->approach_fixes.emplace_back(id, alt_restriction, spd_restriction, distance_to_next, heading);
                }
                break;
            }
            case 3: {
                auto array = value->u.object.values[obj_index].value->u.array;
                for (unsigned int arr_index = 0; arr_index < array.length; arr_index++) {
                    auto current_waypoint = array.values[arr_index];
                    
                    std::string id;
                    long alt_restriction;
                    long spd_restriction;
                    long heading;
                    double distance_to_next;

                    for(unsigned int obj_index = 0; obj_index < current_waypoint->u.object.length; obj_index++) {
                        auto current_value = current_waypoint->u.object.values[obj_index].value;
                        switch (obj_index) {
                        case 0:
                            id = current_value->u.string.ptr;
                            break;
                        case 1:
                            alt_restriction = current_value->u.integer;
                            break;
                        case 2:
                            spd_restriction = current_value->u.integer;
                            break;
                        case 3:
                            heading = current_value->u.integer;
                            break;
                        case 4:
                            distance_to_next = current_value->u.dbl;
                            break;
                        default:
                            printf("Warning: Unexpected property %s in %s.json\n", current_waypoint->u.object.values[obj_index].name, procedure_id.c_str());
                            break;
                        }
                    }

                    this->missed_approach_fixes.emplace_back(id, alt_restriction, spd_restriction, distance_to_next, heading);
                }
                break;
            }
            default: {
                printf("Warning: Unexpected property %s in %s\n", value->u.object.values[obj_index].name, filename.c_str());
                break;
            }
        }
    }

    json_value_free(value);
    free(file_contents);

    return 0;
}
