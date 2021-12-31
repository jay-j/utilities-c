#include "smooth_delay.h"

// Create the structure, allocate interior variables, and initialize the delay size
SmoothDelayInfo profile_smoothdelay_setup(size_t steps){
   SmoothDelayInfo profile;
   profile.step_current = 0;
   profile.steps = steps;

   // init the coefficients. sum(coefficients) = 1
   profile.coefficients = (double*) malloc(profile.steps * sizeof( *profile.coefficients));
   double sum = 0;
   
   // generate unscaled parabola
   for (int i=0; i<profile.steps; ++i){
      double t = (double) i;
      profile.coefficients[i] = -(t + 1)*(t - (double) profile.steps);
      sum += profile.coefficients[i];
   }
   // normalize
   for (int i=0; i<profile.steps; ++i){
      profile.coefficients[i] /= sum;
   }

   // generate the input matrix
   profile.history = (double*) malloc(profile.steps * sizeof( *profile.history));
   for (size_t i=0; i<profile.steps; ++i){
      profile.history[i] = 0;
   }

   return profile;
}


// updates history with the new input, spits out a new filterred value
// CAUTION - relies on the calling loop to operate at a constant frequency
double profile_smoothdelay_smooth(SmoothDelayInfo* profile, double current_raw){
   profile->history[profile->step_current] = current_raw;

   // discrete convolution integral. 
   double result = 0;
   for(int i=0; i<profile->steps; ++i){
      int history_index = i; // (i - profile->step_current) % profile->steps;
      int profile_index = i - profile->step_current - 1;
      if (profile_index >= profile->steps){
         profile_index -= profile->steps;
      }
      if (profile_index < 0){
         profile_index += profile->steps;
      }
      result += profile->coefficients[profile_index] * profile->history[history_index];
   }   

   // advance the current step, wrap as needed
   profile->step_current = (profile->step_current + 1) % profile->steps;

   return result;
}


// cleanup
void profile_smoothdelay_free(SmoothDelayInfo* profile){
   free(profile->coefficients);
   free(profile->history);
}


// Print computed coefficients
void profile_smoothdelay_print_coefficients(SmoothDelayInfo* profile){
   printf("Profile Coefficients: %d\n", profile->steps);
   for(size_t i=0; i<profile->steps; ++i){
      printf("%lf  ", profile->coefficients[i]);
   }
   printf("\n");
}
