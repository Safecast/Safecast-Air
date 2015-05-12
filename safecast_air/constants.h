#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "alphasense_gas_sensor.h"

namespace constants
{
    const int NumGasSensor = 6;
    extern const int GasSensorAinResolution; 
    extern const int GasSensorAinAveraging;
    extern const float GasSensorSampleDt;
    extern const alphasense::GasSensorParam GasSensorParam[];
}


#endif
