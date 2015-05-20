#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "gas_sensor_param.h"
#include "tmp_sensor_param.h"
#include "pm_sensor_param.h"

namespace constants
{
    const int NumGasSensor = 6;
    const int NumTmpSensor = 2;

    extern const int GasSensorAinResolution; 
    extern const int GasSensorAinAveraging;
    extern const unsigned long GasSensorSampleDt;
    extern const GasSensorParam DefaultGasSensorParam[];

    extern const int TmpSensorAinResolution;
    extern const int TmpSensorAinAvaragin;
    extern const unsigned long TmpSensorSampleDt;
    extern const TmpSensorParam DefaultTmpSensorParam[];

    extern const PMSensorParam DefaultPMSensorParam;

}


#endif
