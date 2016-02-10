#include "constants.h" 
#include <Arduino.h>

namespace constants
{
    const char SoftwareVersion[] = "0.32CH4";
    const char DeviceName[] = "air003";
    const char DeviceId[] = "00000003";

    const long USBSerialBaudRate = 115200;
    const unsigned long LoopDelay = 250;

    const int DisplayDC = 5;
    const int DisplayCS = 3;
    const int DisplayReset = 4;
    const SPISettings DisplaySPISettings(4000000,MSBFIRST,SPI_MODE0);

    const MQ4_MethaneParam DefaultMethaneParam = 
    { // priority, sampleDt, ainPin, ainReadRes, ainReadAvg, ainMaxVal, lowPassCutoffFreq, lowPassOrder, numLookup, tableVoltToPPM[MQ4_MaxNumLookup][2], {ids}, 
        100, 50, A17, 16, 32, 1.2, 0.5, 2, 6, {{0.0197, 0.001}, {0.0710, 1.8}, {0.1588, 200.0}, {0.2788, 1000.0}, {0.4770, 5000.0}, {0.6295, 10000.0}}, {107,108,109}
    };

    const LoggerParam DefaultLoggerParam = 
    { // timerPeriod (s), baudRate
        300, 115200 
        //60, 115200 
    };
}


