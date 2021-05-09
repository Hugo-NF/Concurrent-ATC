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
    
    printf("All info loaded!\n");
    approach_ctl.print_info();

    return 0;
}