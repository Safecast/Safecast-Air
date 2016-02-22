#ifndef CONSTANTS_H
#define CONSTANTS_H

// Cheap hack to tide me over until loading configuration from SD card is working
// Really need to fix this.
#define WITH_SEANS_PARAMS 
//#define WITH_ROBS_PARAMS

#define WITH_METHANE

#include "sampling_param.h"
#include "gas_sensor_param.h"
#include "tmp_sensor_param.h"
#include "opcn2_param.h"
#include "opcn2_ids.h"
#include "logger_param.h"
#include <SPI.h>


#ifdef WITH_METHANE
#include "mq4_methane.h"
#endif

namespace constants
{
    extern const char SoftwareVersion[];
    extern const char DeviceName[]; 
    extern const char  DeviceId[]; 

    extern const long USBSerialBaudRate;
    extern const unsigned long LoopDelay;

    // Gas sensor Constatns
#ifdef WITH_SEANS_PARAMS
    const int NumGasSensor = 3;
    const int NumTmpSensor = 1;
#endif

#ifdef WITH_ROBS_PARAMS
    const int NumGasSensor = 6;
    const int NumTmpSensor = 2;
#endif
    
    extern const SamplingParam DefaultGasSensorSamplingParam;
    extern const GasSensorParam DefaultGasSensorParam[];

    // Temperature sensor constants
    extern const SamplingParam DefaultTmpSensorSamplingParam;
    extern const TmpSensorParam DefaultTmpSensorParam[];

    // Particle counter parameters
    extern const OPCN2Param DefaultOPCN2Param;
    extern const OPCN2Ids DefaultOPCN2Ids;

    // OLED Display constatns
    extern const int DisplayDC;
    extern const int DisplayCS;
    extern const int DisplayReset;
    extern const SPISettings DisplaySPISettings;

#ifdef WITH_METHANE
    extern const MQ4_MethaneParam DefaultMethaneParam;
    extern const LoggerParam DefaultLoggerParam;
#endif

    // Logger parameters
    extern const LoggerParam DefaultLoggerParam;

}

#endif
