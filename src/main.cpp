#include <cstdio>
#include <cstdlib>
#include <map>
#include <pthread.h>
#include <string>
#include <unistd.h>


#include "tma.h"


int main(int argc, char **argv) {

    tma approach_ctl;
    approach_ctl.load_from_json("data/tma.json");
    approach_ctl.load_flights("data/flights.json");

    
    printf("Radio frequencies available: %ld\n", frequencies.size());
    approach_ctl.print_info();
    
    return 0;
}