#include "trap.h"
#include <stdio.h>

int approx(double value, double reference){
    if ((value < reference + 0.01) & (value > reference - 0.01)){
        return 1;
    }
    return 0;
}

int main(int argc, const char** argv){
    // setup
    TrapInfo trap = profile_trap_setup(3.0, 1.0);


    double goal = 25;
    double dt = 0.1; 
    double t = 0;

    printf("t,current_position,current_velocity,accelerate,stop_position\n");

    while (t < 40){
        profile_trap_smooth(&trap, goal, dt);
        printf("%lf,%lf,%lf,%lf,0\n", t, trap.current_position, trap.current_velocity, trap.current_acceleration);
     
        if (approx(t, 5)){
            goal = 5;
        }

        if (approx(t, 12.5)){
            goal = 0;
        }

        if (approx(t, 20)){
            goal = 10;
        }
        if (approx(t, 26.5)){
            goal = 15;
        }

        if (approx(t, 33)){
            goal = 10;
        }

        t += dt;
    }

    return 0;
}