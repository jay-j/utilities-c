#include "smooth_delay.h"
#include <stdio.h>

int approx(double value, double reference){
    if ((value < reference + 0.01) & (value > reference - 0.01)){
        return 1;
    }
    return 0;
}

int main(int argc, const char** argv){
    // setup
    SmoothDelayInfo profile = profile_smoothdelay_setup(20);
    profile_smoothdelay_print_coefficients(&profile);

    double goal_raw = 1;
    double goal_smooth;
    int t = 0;


    while (t < 40){
       if (t < 20){
         goal_raw = 1;
      }
      else{
      goal_raw = 0;
      }
        
       goal_smooth = profile_smoothdelay_smooth(&profile, goal_raw);
       printf("%d\t%lf\t%lf\n", t, goal_raw, goal_smooth);
     

        t += 1;
    }

    profile_smoothdelay_free(&profile);

    return 0;
}
