#include <pthread.h>
#include <unistd.h>

#include "../include/aircraft.h"

#include "../include/message_logger.h"

void aircraft::run() {
    info("INIT", "Aircraft running...");
}