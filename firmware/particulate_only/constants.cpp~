#include "constants.h" 
#include <Arduino.h>

namespace constants
{
    const char SoftwareVersion[] = "0.33";
    const char DeviceName[] = "air001";
    const char DeviceId[] = "00000001";

    const long USBSerialBaudRate = 115200;
    const unsigned long LoopDelay = 200;

    // Optical particle coutner OPCN2 parameters  
    const OPCN2Param DefaultOPCN2Param = {750000, MSBFIRST, SPI_MODE1, 24};// spiClock , spiBitOrder, spiDataMode, spiCsPin
    const OPCN2Ids DefaultOPCN2Ids = {29,30, 31, 32, 33, 34, 35, 36, 37, {38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53}};

    // SHT31 Temperature/Humidity sensor parameters
    extern const int SHT31Address = 0x44;

    // OLED Dislay settings
    const int DisplayDC = 5;
    const int DisplayCS = 3;
    const int DisplayReset = 4;
    const SPISettings DisplaySPISettings(4000000,MSBFIRST,SPI_MODE0);

    // Logger params 
    // timerPeriod (s), baudRate 
    //const LoggerParam DefaultLoggerParam = {60, 115200};
    const LoggerParam DefaultLoggerParam = {15, 115200};
}


