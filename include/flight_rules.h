
enum flight_phases {
    ON_GROUND,
    TAXIING,
    WAITING_CLEARANCE,
    TAKEOFF,
    CLIMBING,
    CRUISING,
    DESCENDING,
    APPROACH,
    LANDING,
    ROLLOUT
};

enum message_types {
    TAXI_REQUEST,
    TAXI_CLEARANCE,
    TAKEOFF_REQUEST,
    TAKEOFF_CLEARANCE,
    LANDING_REQUEST,
    LANDING_CLEARANCE,
    GO_AROUND_REQUEST,
    ENTER_HOLDING_REQUEST,
    EXIT_HOLDING_REQUEST,
    EXIT_HOLDING_CLEARANCE
};