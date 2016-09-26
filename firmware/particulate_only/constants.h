#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "opcn2_param.h"
#include "opcn2_ids.h"
#include "openlog_param.h"
#include "logger_param.h"
#include <SPI.h>


namespace constants
{
    extern const String SoftwareVersion;
    extern const String DeviceName; 
    extern const String ConfigurationFile;

    extern const long USBSerialBaudRate;
    extern const unsigned long LoopDelay;

    // Particle counter parameters
    extern const OPCN2Param DefaultOPCN2Param;
    extern const OPCN2Ids DefaultOPCN2Ids;

    // SHT31 Temperature/Humidity sensor parameters
    extern const int SHT31Address;

    // OLED Display constatns
    extern const int DisplayDC;
    extern const int DisplayCS;
    extern const int DisplayReset;
    extern const SPISettings DisplaySPISettings;

    // Openlog parameters
    extern const OpenlogParam DefaultOpenlogParam;

    // Load configuration parameters
    extern const int NumLoadConfigTrys;

    // Logger parameters
    extern const LoggerParam DefaultLoggerParam;
}

#endif
