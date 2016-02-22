#include "constants.h" 
#include "gas_sensor.h"
#include <Arduino.h>

namespace constants
{
    const char SoftwareVersion[] = "0.33";
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

    // Temperature sensor parameters
    // --------------------------------------------------------------------------------------------
    const SamplingParam DefaultTmpSensorSamplingParam = 
    {
        60, 16, 32, 50
    };

#ifdef WITH_ROBS_PARAMS
    // Rob's prototype
    // --------------------------------------------------------------------------------------------
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

    const TmpSensorParam DefaultTmpSensorParam[NumTmpSensor] = 
    { // header, ainPin, offset, sensitivity, ainVRef, ainScaleFact, lowPassCutoffFreq, lowPassOrder, active, {ids}
        {0, A13, 0.5, 10.0, 1.2, 1.0, 0.005, 3, true, {24,25}},
        {1, A12, 0.5, 10.0, 1.2, 1.0, 0.005, 3, true, {26,27}}
    };

    const OPCN2Ids DefaultOPCN2Ids = 
    {
        29,30, 31, 32, 33, 34, 35, 36, 37, {38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53}
    };
#endif

#ifdef WITH_SEANS_PARAMS
    // Sean's prototype
    // ------------------------------------------------------------------------------------------
    const GasSensorParam DefaultGasSensorParam[NumGasSensor] = 
    {   // gasType, serialNumber, header, position, workingAinPin, auxillaryAinPin, workingZero, auxillaryZero,
        // sensitivity, ainVRef, ainScaleFact, powerScaleFact, lowPassCutoffFreq, lowPassOrder, active, {ids}
        {GAS_TYPE_NO2, 131410831ul, 0, 0, A0,  A1,  0.292,  0.319,  0.357,  1.2,  4.4,  1.00,  0.005,  3, true, {54,  55, 56,  57}}, 
        {GAS_TYPE_O3,  135410519ul, 0, 1, A2,  A3,  0.410,  0.415,  0.183,  1.2,  4.4,  1.00,  0.005,  3, true, {58,  59, 60,  61}},
        {GAS_TYPE_CO,  132410527ul, 0, 2, A4,  A5,  0.326,  0.270,  0.468,  1.2,  4.4,  1.00,  0.005,  3, true, {62,  63, 64,  65}}
        //{GAS_TYPE_NO2, 131410833ul, 1, 0, A6,  A7,  0.297,  0.307,  0.335,  1.2,  4.4,  1.00,  0.005,  3, true, {66,  67, 68,  69}},
        //{GAS_TYPE_O3,  135410523ul, 1, 1, A8,  A9,  0.396,  0.416,  0.225,  1.2,  4.4,  1.00,  0.005,  3, true, {70,  71, 72,  73}},
        //{GAS_TYPE_CO,  132410528ul, 1, 2, A10, A11, 0.436,  0.329,  0.532,  1.2,  4.4,  1.00,  0.005,  3, true, {74,  75, 76,  77}}
    };

    // Sean's prototype
    const TmpSensorParam DefaultTmpSensorParam[NumTmpSensor] = 
    { // header, ainPin, offset, sensitivity, ainVRef, ainScaleFact, lowPassCutoffFreq, lowPassOrder, active, {ids}
        {0, A13, 0.3009, 1.0, 1.2, 1.0, 0.005, 3, true, {78, 79}}
        //{1, A12, 0.3009, 1.0, 1.2, 1.0, 0.005, 3, true, {80, 81}}
    };

    // Seans Proto
    const OPCN2Ids DefaultOPCN2Ids = 
    {
        82, 83, 84, 85, 86, 87, 88, 89, 90, {91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106}
    };
#endif

    // Optical particle coutner OPCN2 parameters
    // --------------------------------------------------------------------------------------------
    const OPCN2Param DefaultOPCN2Param = 
    {   // spiClock , spiBitOrder, spiDataMode, spiCsPin
        750000, MSBFIRST, SPI_MODE1, 24                
    };

    
#ifdef WITH_METHANE
    const MQ4_MethaneParam DefaultMethaneParam = 
    { // priority, sampleDt, ainPin, ainReadRes, ainReadAvg, ainMaxVal, lowPassCutoffFreq, lowPassOrder, numLookup, tableVoltToPPM[MQ4_MaxNumLookup][2], {ids}, 
        100, 50, A17, 16, 32, 1.2, 0.5, 2, 6, {{0.04264, 0.001},{0.14890, 1.8},{0.32673, 200.0},{0.55889, 1000.0},{0.91745, 5000.0}, {1.17427, 10000.0}}, {110,111,112}
    };
#endif

    // OLED Dislay settings
    // --------------------------------------------------------------------------------------------
    const int DisplayDC = 5;
    const int DisplayCS = 3;
    const int DisplayReset = 4;
    const SPISettings DisplaySPISettings(4000000,MSBFIRST,SPI_MODE0);

    // Logger params
    // ---------------------------------------------------------------------------------------------
    const LoggerParam DefaultLoggerParam = 
    { //  timerPeriod (s), baudRate 
        60, 115200 
    };
}


