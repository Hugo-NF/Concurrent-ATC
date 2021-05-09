
class threadable {
    
public:
    virtual void* run(void* thread_id) = 0;
    virtual void load_from_json(const char* filename) = 0;
    virtual void print_info() = 0;
};