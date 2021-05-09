#include "../include/waypoint.h"

void sid::load_from_json(std::string procedure_id) {
    this->id = procedure_id;
}

void star::load_from_json(std::string procedure_id) {
    this->id = procedure_id;
}

void approach::load_from_json(std::string procedure_id) {
    this->id = procedure_id;
}
