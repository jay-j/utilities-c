#include "trap.h"
#include "smooth_delay.h"
#include "profile_ema.h"
#include <stdio.h>

int approx(double value, double reference){
    if ((value < reference + 0.01) & (value > reference - 0.01)){
        return 1;
    }
    return 0;
}

int main(int argc, const char** argv){
    double goal = 25;
    double dt = 0.1; 
    double t = 0;

    // setup
    TrapInfo trap = profile_trap_setup(3.3, 1.0);
    SmoothDelayInfo smoothdelay = profile_smoothdelay_setup(40);
    EMAInfo ema = profile_ema_setup(dt, 3.5);

    printf("t,goal,trap,delay,ema\n");
    
    double sd_position = 0;
    double ema_position = 0;

    while (t < 40){
        profile_trap_smooth(&trap, goal, dt);
        sd_position = profile_smoothdelay_smooth(&smoothdelay, goal);
        ema_position = profile_ema_update(&ema, goal);

        printf("%lf,%lf,%lf,%lf,%lf\n", t, goal, trap.current_position, sd_position, ema_position);
     
        if (approx(t, 7)){
            goal = 5;
        }

        if (approx(t, 10)){
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
