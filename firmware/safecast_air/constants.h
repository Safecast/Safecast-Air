#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "sampling_param.h"
#include "gas_sensor_param.h"
#include "tmp_sensor_param.h"
#include "opcn2_param.h"
#include "logger_param.h"
#include <SPI.h>

namespace constants
{
    extern const char DeviceName[]; 
    extern const char  DeviceId[]; 

    extern const long USBSerialBaudRate;
    extern const unsigned long LoopDelay;

    // Gas sensor Constatns
    const int NumGasSensor = 6;
    extern const SamplingParam DefaultGasSensorSamplingParam;
    extern const GasSensorParam DefaultGasSensorParam[];

    // Temperature sensor constants
    const int NumTmpSensor = 2;
    extern const SamplingParam DefaultTmpSensorSamplingParam;
    extern const TmpSensorParam DefaultTmpSensorParam[];

    // Particle counter parameters
    extern const OPCN2Param DefaultOPCN2Param;

    // OLED Display constatns
    extern const int DisplayDC;
    extern const int DisplayCS;
    extern const int DisplayReset;
    extern const SPISettings DisplsySPISettings;

    // Logger parameters
    extern const LoggerParam DefaultLoggerParam;

}

#endif
