#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "sampling_param.h"
#include "gas_sensor_param.h"
#include "tmp_sensor_param.h"
#include "opcn2_param.h"
///#include "amphenol_pm_sensor_param.h"
//#include "opcn2_param.h"

namespace constants
{
    const int NumGasSensor = 6;
    extern const SamplingParam GasSensorSamplingParam;
    extern const GasSensorParam DefaultGasSensorParam[];

    const int NumTmpSensor = 2;
    extern const SamplingParam TmpSensorSamplingParam;
    extern const TmpSensorParam DefaultTmpSensorParam[];

    extern const OPCN2Param DefaultOPCN2Param;
}

#endif
