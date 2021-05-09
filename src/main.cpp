#include <cstdio>
#include <cstdlib>
#include <map>
#include <pthread.h>
#include <string>
#include <unistd.h>


#include "tma.h"


int main(int argc, char **argv) {

    tma approach_ctl;
    pthread_t approach_ctl_thread;

    approach_ctl.load_from_json("data/tma.json");
    approach_ctl.load_flights("data/flights.json");

    
    printf("Radio frequencies available: %ld\n", frequencies.size());
    approach_ctl.print_info();

    printf("\nFlights loaded: %ld\n", approach_ctl.flights_on_terminal.size());
    for(auto it = approach_ctl.flights_on_terminal.begin(); it != approach_ctl.flights_on_terminal.end(); ++it) {
        it.operator*().print_info();
    }
    
    printf("Confira as informações acima e aperte uma tecla para iniciar...\n");
    getchar();

    // Start TMA thread
    pthread_create(&approach_ctl_thread, NULL, approach_ctl.run, (void *) &approach_ctl);
    
    // Waiting thread to finish
    pthread_join(approach_ctl_thread, 0);

    return 0;
}