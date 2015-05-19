#ifndef GAS_SENSOR_PARAM_H
#define GAS_SENSOR_PARAM_H

enum GasType 
{
    GAS_TYPE_NONE, 
    GAS_TYPE_NO2, 
    GAS_TYPE_NO, 
    GAS_TYPE_O3, 
    GAS_TYPE_CO, 
    GAS_TYPE_H2S, 
    GAS_TYPE_SO2
};

struct GasSensorParam
{
    GasType gasType;
    int workingAinPin;
    int auxillaryAinPin;
    float workingZero;
    float auxillaryZero;
    float sensitivity;
    float ainScaleFact;
    float powerScaleFact;
    float lowPassCutoffFreq;
    unsigned int lowPassOrder;
    int temperatureSensor;
};

#endif
