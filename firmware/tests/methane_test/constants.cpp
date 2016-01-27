#include "constants.h" 
#include <Arduino.h>

namespace constants
{
    const char SoftwareVersion[] = "0.32CH4";
    const char DeviceName[] = "air003";
    const char DeviceId[] = "00000003";

    const long USBSerialBaudRate = 115200;
    const unsigned long LoopDelay = 50;

    const int DisplayDC = 5;
    const int DisplayCS = 3;
    const int DisplayReset = 4;
    const SPISettings DisplaySPISettings(4000000,MSBFIRST,SPI_MODE0);

    const MQ4_MethaneParam DefaultMethaneParam = 
    { // priority, sampleDt, ainPin, ainReadRes, ainReadAvg, ainMaxVal, lowPassCutoffFreq, lowPassOrder, numLookup, tableVoltToPPM[MQ4_MaxNumLookup][2], {ids}, 
        100, 50, A17, 16, 32, 1.2, 0.5, 2, 5, {{0.0720, 1.8}, {0.1610, 200.0}, {0.2826, 1000.0}, {0.4832, 5000.0}, {0.6374, 10000.0}}, {107,108,109}
    };

    const LoggerParam DefaultLoggerParam = 
    { // timerPriority,  timerPeriod (us), baudRate
        122, 10000000, 115200 
    };
}


