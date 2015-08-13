#include "constants.h" 
#include "gas_sensor.h"
#include <Arduino.h>

namespace constants
{
    // Gas Sensor parameters
    // --------------------------------------------------------------------------------------------
    const SamplingParam GasSensorSamplingParam = 
    {   // timerPriority,  ainResolution;, ainAveraging,  sampleDt; 
        100, 16, 32, 50 
    };

    const GasSensorParam DefaultGasSensorParam[NumGasSensor] = 
    {   // gasType, serialNumber, workingAinPin, auxillaryAinPin, workingZero, auxillaryZero, sensitivity, 
        // ainVRef, ainScaleFact, powerScaleFact, lowPassCutoffFreq, lowPassOrder, active
        
        {GAS_TYPE_NO2, 161740024ul, A0,  A1,  0.234,  0.235,  0.435,  1.2,  4.4,  1.00,  0.1,  3, true}, 
        {GAS_TYPE_O3,  165417321ul, A2,  A3,  0.416,  0.416,  0.321,  1.2,  4.4,  1.00,  0.1,  3, true},
        {GAS_TYPE_CO,  162740011ul, A4,  A5,  0.398,  0.346,  0.403,  1.2,  4.4,  1.00,  0.1,  3, true},
        {GAS_TYPE_NO2, 161740022ul, A6,  A7,  0.226,  0.226,  0.374,  1.2,  4.4,  1.00,  0.1,  3, true},
        {GAS_TYPE_O3,  165417320ul, A8,  A9,  0.405,  0.417,  0.283,  1.2,  4.4,  1.00,  0.1,  3, true},
        {GAS_TYPE_CO,  162740010ul, A10, A11, 0.406,  0.363,  0.381,  1.2,  4.4,  1.00,  0.1,  3, true}
    };

    // Temperature sensor parameters
    // --------------------------------------------------------------------------------------------
    const SamplingParam TmpSensorSamplingParam = 
    {
        100, 16, 32, 50
    };

    const TmpSensorParam DefaultTmpSensorParam[NumTmpSensor] = 
    {
        {A13, 0.5, 10.0, 1.2, 1.0, 0.1, 3, true},
        {A12, 0.5, 10.0, 1.2, 1.0, 0.1, 3, true}
    };

    // Optical particle coutner OPCN2 parameters
    // --------------------------------------------------------------------------------------------
    const OPCN2Param DefaultOPCN2Param = 
    {   // spiClock , spiBitOrder, spiDataMode, spiCsPin
        750000, MSBFIRST, SPI_MODE1, 24                
    };
}


//const OPCN2Param DefaultOPCN2Param2 = 
//{   // spiClock , spiBitOrder, spiDataMode, spiCsPin
//    750000, MSBFIRST, SPI_MODE1, 28                
//};
