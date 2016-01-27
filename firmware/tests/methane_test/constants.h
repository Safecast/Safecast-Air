#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "logger_param.h"
#include "mq4_methane.h"
#include <SPI.h>

namespace constants
{
    extern const char SoftwareVersion[];
    extern const char DeviceName[]; 
    extern const char  DeviceId[]; 

    extern const long USBSerialBaudRate;
    extern const unsigned long LoopDelay;

    extern const int DisplayDC;
    extern const int DisplayCS;
    extern const int DisplayReset;
    extern const SPISettings DisplaySPISettings;

    extern const MQ4_MethaneParam DefaultMethaneParam;
    extern const LoggerParam DefaultLoggerParam;
}

#endif
