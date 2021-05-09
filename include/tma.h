#ifndef TMA_H
#define TMA_H

#include "threadable.h"

class tma: public threadable {

public:
    void* run(void* thread_id);
    void load_from_json(const char* filename);
    void print_info();

};

#endif // TMA_H