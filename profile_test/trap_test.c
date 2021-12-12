#include "trap.h"
#include <stdio.h>


int main(int argc, const char** argv){
    // setup
    TrapInfo trap = profile_trap_setup(3.0, 0.5);


    double goal = 20;
    double dt = 0.5; 
    double t = 0;

    while (t < 30){
        printf("\nt=%lf\n", t);
        profile_trap_smooth(&trap, goal, dt);

        t += dt;
    }

    return 0;
}