#ifndef FLIGHT_RULES_H
#define FLIGHT_RULES_H

enum flight_phases {
    ON_GROUND,
    TAXIING,
    WAITING_CLEARANCE,
    TAKEOFF,
    CLIMBING,
    CLIMBING_VFR,
    CRUISING,
    DESCENDING,
    DESCENDING_VFR,
    APPROACH,
    LANDING,
    ROLLOUT,
    GO_AROUND,
    HOLDING,
    MAYDAY,
    PANPAN
};

enum message_types {
    CHECK_IN,
    CHECK_OUT,
    TAXI_REQUEST,
    TAXI_CLEARANCE,
    DESCEND_REQUEST,
    DESCEND_CLEARANCE,
    DESCEND_VFR_CLEARANCE,
    CLEARANCE_DENY,
    CLIMBING_REQUEST,
    CLIMBING_CLEARANCE,
    CLIMBING_VFR_CLEARANCE,
    TAKEOFF_REQUEST,
    TAKEOFF_CLEARANCE,
    LANDING_REQUEST,
    LANDING_CLEARANCE,
    AFTER_TAKEOFF,
    AFTER_LANDING,
    AFTER_GO_AROUND,
    ENTER_HOLDING_REQUEST,
    EXIT_HOLDING_REQUEST,
    EXIT_HOLDING_CLEARANCE,
    MAYDAY_CALL,
    PANPAN_CALL,
    MAYDAY_ASSISTANCE,
    PANPAN_ASSISTANCE
};

class flight_rules {

public:
    static const char* get_flight_phase(int type) {
        switch (type) {
        case ON_GROUND:
            return "GROUND";
        case TAXIING:
            return "TAXIING";
        case WAITING_CLEARANCE:
            return "WAITING CLEARANCE FOR";
        case TAKEOFF:
            return "TAKE-OFF";
        case CLIMBING:
            return "CLIMB";
        case CRUISING:
            return "CRUISE";
        case DESCENDING:
            return "DESCEND";
        case APPROACH:
            return "APPROACH";
        case LANDING:
            return "LANDING";
        case ROLLOUT:        
            return "ROLLOUT";
        default:
            return "UNKNOWN";
        }
    }
};

#endif // FLIGHT_RULES_H