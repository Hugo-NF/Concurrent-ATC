#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <string>
#include <vector>

class waypoint {
public:
    std::string id;
    long alt_restriction;
    long spd_restriction;
    long heading;
    long distance_to_next;

    explicit waypoint(
        std::string id,
        long alt_restriction = 0,
        long spd_restriction = 0,
        long heading = 0,
        long distance_to_next = 0
    ) {
        this->id = id;
        this->alt_restriction = alt_restriction;
        this->spd_restriction = spd_restriction;
        this->heading = heading;
        this->distance_to_next = distance_to_next;
    }

    virtual ~waypoint() = default;
};

class sid {
public:
    std::string id;
    std::vector<waypoint> fixes;

    explicit sid() = default;
    virtual ~sid() = default;

    void load_from_json(std::string procedure_id);
};

class star {
public:    
    std::string id;
    std::vector<waypoint> fixes;

    explicit star() = default;
    virtual ~star() = default;

    void load_from_json(std::string procedure_id);
};

class approach {
public:
    std::string id;
    std::vector<waypoint> approach_fixes;
    std::vector<waypoint> missed_approach_fixes;

    explicit approach() = default;
    virtual ~approach() = default;

    void load_from_json(std::string procedure_id);
};

#endif // WAYPOINT_H