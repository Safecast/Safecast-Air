#ifndef TMP_SENSOR_PARAM_H
#define TMP_SENSOR_PARAM_H

struct TmpSensorParam
{
    int ainPin;
    float offset;
    float sensitivity;
    float ainVRef;
    float ainScaleFact;
    float lowPassCutoffFreq;
    unsigned int lowPassOrder;
    bool active;
};

#endif
