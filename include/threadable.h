
class threadable {
    
public:
    virtual void* run(void* thread_id) = 0;
    virtual void print_info() = 0;
};