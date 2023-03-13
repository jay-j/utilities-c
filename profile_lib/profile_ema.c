#include "profile_ema.h"
#include <math.h>

EMAInfo profile_ema_setup(double dt, double time){
  EMAInfo ema = {0};

  // calculate alpha based on 63% time constant and expected dt
  ema.alpha = 1.0 - exp(- 3 * dt / time);

  return ema;
}



double profile_ema_update(EMAInfo* ema, double goal){
  ema->value = (1-ema->alpha)*ema->value + ema->alpha*goal;
  return ema->value;
}
