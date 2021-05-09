#include <cstdio>
#include <cstdlib>
#include <map>
#include <pthread.h>
#include <string>
#include <unistd.h>

#include "flight_rules.h"
#include "radio.h"
#include "aircraft.h"

int main(int argc, char **argv) {

    aircraft b738 = aircraft(280, 210, 147, 165, 230, 260, 7545, 5249, 210, 18000);

    printf("Distance to next waypoint: %d\n", b738.calculate_next_waypoint(10, 0.9));

    return 0;
}