#ifndef TMP_SENSOR_H
#define TMP_SENSOR_H

struct TmpSensorParam
{
    int ainPin;
    float sensitivity;
    float offsetVal;
    float offsetTmp;
    float lowPassCutoffFreq;
    unsigned int lowPassOrder;
};

#endif
