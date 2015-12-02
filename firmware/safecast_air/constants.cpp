#include "constants.h" 
#include "gas_sensor.h"
#include <Arduino.h>

namespace constants
{
    const char DeviceName[] = "air001";
    const char DeviceId[] = "00000001";

    const long USBSerialBaudRate = 115200;
    const unsigned long LoopDelay = 200;

    // Gas Sensor parameters
    // --------------------------------------------------------------------------------------------
    const SamplingParam DefaultGasSensorSamplingParam = 
    {   // timerPriority,  ainResolution;, ainAveraging,  sampleDt; 
        50, 16, 32, 50 
    };

    const GasSensorParam DefaultGasSensorParam[NumGasSensor] = 
    {   // gasType, serialNumber, header, position, workingAinPin, auxillaryAinPin, workingZero, auxillaryZero,
        // sensitivity, ainVRef, ainScaleFact, powerScaleFact, lowPassCutoffFreq, lowPassOrder, active, {ids}
        {GAS_TYPE_NO2, 161740024ul, 0, 0, A0,  A1,  0.234,  0.235,  0.435,  1.2,  4.4,  1.00,  0.005,  3, true, { 0,  1,  2,  3}}, 
        {GAS_TYPE_O3,  165417321ul, 0, 1, A2,  A3,  0.416,  0.416,  0.321,  1.2,  4.4,  1.00,  0.005,  3, true, { 4,  5,  6,  7}},
        {GAS_TYPE_CO,  162740011ul, 0, 2, A4,  A5,  0.398,  0.346,  0.403,  1.2,  4.4,  1.00,  0.005,  3, true, { 8,  9, 10, 11}},
        {GAS_TYPE_NO2, 161740022ul, 1, 0, A6,  A7,  0.226,  0.226,  0.374,  1.2,  4.4,  1.00,  0.005,  3, true, {12, 13, 14, 15}},
        {GAS_TYPE_O3,  165417320ul, 1, 1, A8,  A9,  0.405,  0.417,  0.283,  1.2,  4.4,  1.00,  0.005,  3, true, {16, 17, 18, 19}},
        {GAS_TYPE_CO,  162740010ul, 1, 2, A10, A11, 0.406,  0.363,  0.381,  1.2,  4.4,  1.00,  0.005,  3, true, {20, 21, 22, 23}}
    };

    // Temperature sensor parameters
    // --------------------------------------------------------------------------------------------
    const SamplingParam DefaultTmpSensorSamplingParam = 
    {
        60, 16, 32, 50
    };

    const TmpSensorParam DefaultTmpSensorParam[NumTmpSensor] = 
    { // header, ainPin, offset, sensitivity, ainVRef, ainScaleFact, lowPassCutoffFreq, lowPassOrder, active, {ids}
        {0, A13, 0.5, 10.0, 1.2, 1.0, 0.005, 3, true, {24,25}},
        {1, A12, 0.5, 10.0, 1.2, 1.0, 0.005, 3, true, {26,27}}
    };

    // Optical particle coutner OPCN2 parameters
    // --------------------------------------------------------------------------------------------
    const OPCN2Param DefaultOPCN2Param = 
    {   // spiClock , spiBitOrder, spiDataMode, spiCsPin
        750000, MSBFIRST, SPI_MODE1, 24                
    };

    const OPCN2Ids DefaultOPCN2Ids = 
    {
        29,30, 31, 32, 33, 34, 35, 36, 37, {38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53}
    };

    // OLED Dislay settings
    // --------------------------------------------------------------------------------------------
    const int DisplayDC = 5;
    const int DisplayCS = 3;
    const int DisplayReset = 4;
    const SPISettings DisplsySPISettings(4000000,MSBFIRST,SPI_MODE0);

    // Logger parameters
    // --------------------------------------------------------------------------------------------
    //const LoggerParam DefaultLoggerParam = 
    //{ // timerPriority,  timerPeriod (us), baudRate
    //    122, 60000000, 9600
    //};

    const LoggerParam DefaultLoggerParam = 
    { // timerPriority,  timerPeriod (us), baudRate
        122, 5000000, 9600
    };
}


