#include <cstdio>
#include <cstdlib>
#include <map>
#include <pthread.h>
#include <string>
#include <unistd.h>

#include "flight_rules.h"

int main(int argc, char **argv) {

    std::map<std::string, int> aircrafts;

    aircrafts["LA3419"] = CRUISING;
    aircrafts["GLO3911"] = ON_GROUND;

    std::string origin = "GLO3911";
    
    printf("[%s] is on %s\n", origin.c_str(), flight_rules::get_flight_phase(aircrafts[origin]));

    return 0;
}