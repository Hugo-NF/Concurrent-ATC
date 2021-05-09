#ifndef THREADABLE_H
#define THREADABLE_H

class threadable {
    
public:
    virtual void* run(void* thread_id) = 0;
    virtual void print_info() = 0;
};

#endif //THREADABLE_H