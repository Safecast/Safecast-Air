#include "constants.h" 
#include <Arduino.h>

namespace constants
{
    const String SoftwareVersion = "0.1";
    const String  DeviceName = "air001"; 
    const String ConfigurationFile = "SAFECAST.txt";

    const long USBSerialBaudRate = 115200;
    const unsigned long LoopDelay = 200;

    // Optical particle coutner OPCN2 parameters  
    // spiClock , spiBitOrder, spiDataMode, spiCsPin
    const OPCN2Param DefaultOPCN2Param = {750000, MSBFIRST, SPI_MODE1, 24};
    const OPCN2Ids DefaultOPCN2Ids = {0, 0, 0, 0, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    // SHT31 Temperature/Humidity sensor parameters
    extern const int SHT31Address = 0x44;

    // OLED Dislay settings
    const int DisplayDC = 5;
    const int DisplayCS = 3;
    const int DisplayReset = 4;
    const SPISettings DisplaySPISettings(4000000,MSBFIRST,SPI_MODE0);

    // Openlog parameters
    const OpenlogParam DefaultOpenlogParam = {115200, 500, 0, &Serial3} ;

    // Load configuration parameters
    extern const int NumLoadConfigTrys = 10;

    // Logger parameters 
    // writeLogPeriod(s), dataSamplePeriod(s), baudRate 
    const LoggerParam DefaultLoggerParam = {300, 60, 115200};
    //const LoggerParam DefaultLoggerParam = {30, 15, 115200}; // Testing
}


