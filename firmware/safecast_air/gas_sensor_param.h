#ifndef GAS_SENSOR_PARAM_H
#define GAS_SENSOR_PARAM_H
#include <Arduino.h>

enum GasType 
{
    GAS_TYPE_NONE = 0, 
    GAS_TYPE_NO2, 
    GAS_TYPE_NO, 
    GAS_TYPE_O3, 
    GAS_TYPE_CO, 
    GAS_TYPE_H2S, 
    GAS_TYPE_SO2,
    NUM_GAS_TYPE
};

const String GasTypeToGasName[NUM_GAS_TYPE] = {"None", "NO2", "NO", "O3", "CO", "H2S", "SO2"};

struct GasSensorParam
{
    GasType gasType;
    unsigned long serialNumber;
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
    bool active;
};

#endif
