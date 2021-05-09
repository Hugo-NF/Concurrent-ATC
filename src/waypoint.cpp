#include "../include/waypoint.h"

int sid_star::load_from_json(std::string procedure_id) {
    this->id = procedure_id;
    
    return 0;
}


int approach::load_from_json(std::string procedure_id) {
    this->id = procedure_id;

    return 0;
}
