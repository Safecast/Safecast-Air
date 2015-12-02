#ifndef TMP_SENSOR_PARAM_H
#define TMP_SENSOR_PARAM_H
#include <Arduino.h>

struct TmpSensorIds
{
    unsigned int val;
    unsigned int valFlt;
};

struct TmpSensorParam
{
    uint8_t header;
    int ainPin;
    float offset;
    float sensitivity;
    float ainVRef;
    float ainScaleFact;
    float lowPassCutoffFreq;
    unsigned int lowPassOrder;
    bool active;
    TmpSensorIds ids;
};

#endif
