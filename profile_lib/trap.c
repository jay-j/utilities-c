#include "trap.h"

TrapInfo profile_trap_setup(double profile_speed, double profile_acceleration){
    TrapInfo trap;
    trap.profile_speed = profile_speed;
    trap.profile_acceleration = profile_acceleration;
    trap.current_velocity = 0;
    trap.current_position = 0;
    return trap;
}

inline static double profile_trap_sign(double value){
    return (1.0 - 2.0*(value < 0));
}

inline static double profile_trap_abs(double value){
    return value*profile_trap_sign(value);
}

// TODO write branchless variant
inline static double profile_trap_limit(double value, double low, double high){
    value = value > high ? high : value;
    value = value < low ? low : value;
    return value;
}

double profile_trap_smooth(TrapInfo* trap, double goal, double dt){
    // distance and direction to get to end goal
    double position_error = goal - trap->current_position; 
    double direction_to_goal = profile_trap_sign(position_error);
    printf("position_error: %lf\tdirection_to_goal: %lf\n", position_error, direction_to_goal);

    // signed difference between current velocity target and desired cruise to new goal
    // double velocity_error = profile_trap_sign(position_error)*trap->profile_speed - trap->current_velocity;

    // where do you end up if you try to stop right now?  
    double stop_time = profile_trap_abs(trap->current_velocity) / trap->profile_acceleration;
    double stop_position = 0.5*(-1.0*direction_to_goal*trap->profile_acceleration)*stop_time*stop_time
         + trap->current_velocity*stop_time + trap->current_position;
    double stop_position_error = stop_position - goal;
    printf("stop time: %lf\tstop_position: %lf\n", stop_time, stop_position);

    double accelerate = -1; // deceleration/braking
    // if you fall short of the goal, then accelerate/cruise
    if (stop_position_error*direction_to_goal < 0){
        accelerate = 1;
        printf("need to ACCELERATE\n");
    }
    else{
        printf("need to BRAKE\n");
    }

    // adjust velocity, limited
    trap->current_velocity = profile_trap_limit(trap->current_velocity + trap->profile_acceleration*direction_to_goal*accelerate*dt, 
        -trap->profile_speed, trap->profile_speed);
 
    // increment position target
    trap->current_position = trap->current_position + trap->current_velocity*dt;

    printf("new position: %lf\tnew velocity: %lf\n", trap->current_position, trap->current_velocity);

    // TODO improved stick to goal if get very close at sufficiently low speed

    return trap->current_position;
}
