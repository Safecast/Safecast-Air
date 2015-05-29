#ifndef SAMPLING_PARAM_H
#define SAMPLING_PARAM_H
#include <Arduino.h>

struct SamplingParam
{
    uint8_t timerPriority;
    int ainResolution;
    int ainAveraging;
    unsigned long sampleDt;
};

#endif

