#include "threadable.h"


#include <pthread.h>
#include <unistd.h>

class aircraft: public threadable {

public:
    void* run(void* thread_id);

    explicit aircraft() = default;
    virtual ~aircraft() = default;
};