#ifndef PROFILE_EMA_H
#define PROFILE_EMA_H

// larger alpha uses more of the new values
typedef struct {
  double alpha;
  double value;
} EMAInfo;

EMAInfo profile_ema_setup(double dt, double time);
double profile_ema_update(EMAInfo* ema, double goal);

#endif 