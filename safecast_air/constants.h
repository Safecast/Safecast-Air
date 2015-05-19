#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "gas_sensor_param.h"
#include "pm_sensor_param.h"

namespace constants
{
    const int NumGasSensor = 6;
    extern const int GasSensorAinResolution; 
    extern const int GasSensorAinAveraging;
    extern const float GasSensorSampleDt;
    extern const GasSensorParam DefaultGasSensorParam[];
    extern const PMSensorParam DefaultPMSensorParam;
}


#endif
