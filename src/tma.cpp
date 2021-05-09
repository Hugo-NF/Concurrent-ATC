#include "../include/tma.h"

void* tma::run(void* thread_id){

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

void tma::print_info(){
    printf("Terminal loaded: TMA %s\n", this->id.c_str());
    printf("\tDescription: %s\n", this->description.c_str());
    printf("\tRadio frequency: %lf\n", this->radio_frequency);
    
    printf("\tAirports:\n");
    for(auto it = airports.begin(); it != airports.end(); ++it) {
        auto airport = it.operator*();
        printf("\t\t%s:\n", airport.first.c_str());
        airport.second.print_info();
    }
}
