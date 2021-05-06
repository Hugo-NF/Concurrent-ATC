#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <sys/stat.h>

#include "message_logger.h"
#include "aircraft.h"



int main(int argc, char **argv) {

    aircraft latam3419 = aircraft();

    latam3419.run();

    return 0;
}