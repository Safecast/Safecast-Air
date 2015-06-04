#ifndef UTILITY_H 
#define UTILITY_H
#include <limits.h>

inline float mapFloat(float x, float inMin, float inMax, float outMin, float outMax)
{
      return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

inline unsigned long getDt(unsigned long t0, unsigned long t1)
{
    unsigned long dt = 0;
    if (t1 < t0)
    {
        dt = t1 + (ULONG_MAX - t0);
    }
    else
    {
        dt = t1 - t0;
    }
    return dt;
}

#endif
