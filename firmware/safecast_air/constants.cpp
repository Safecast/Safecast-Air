#include "constants.h" 
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
        // ainScaleFact, powerScaleFact, lowPassCutoffFreq, lowPassOrder, temperatureSensor, active
        {GAS_TYPE_NO2, 131410834ul, A0,  A1,  0.282,  0.296,  0.305,  4.4,  1.0,  0.1,  3, 0, true}, 
        {GAS_TYPE_O3,  135410521ul, A2,  A3,  0.422,  0.407,  0.235,  4.4,  1.0,  0.1,  3, 0, true},
        {GAS_TYPE_CO,  132410526ul, A4,  A5,  0.272,  0.250,  0.497,  4.4,  1.0,  0.1,  3, 0, true},
        {GAS_TYPE_NO2, 131410831ul, A6,  A7,  0.292,  0.319,  0.357,  4.4,  1.0,  0.1,  3, 1, false},
        {GAS_TYPE_O3,  135410519ul, A8,  A9,  0.410,  0.415,  0.183,  4.4,  1.0,  0.1,  3, 1, false},
        {GAS_TYPE_CO,  132410527ul, A10, A11, 0.326,  0.270,  0.468,  4.4,  1.0,  0.1,  3, 1, false}
    };
    
    //  Temperature sensor parameters
    //  -------------------------------------------------------------------------------------------
    
    const SamplingParam TmpSensorSamplingParam = 
    {   // timerPriority,  ainResolution;, ainAveraging,  sampleDt; 
        200, 16, 32, 100 
    };
    const TmpSensorParam DefaultTmpSensorParam[NumTmpSensor] = 
    {  // ainPin, sensitivity, offsetVal,, offsetTmp,  lowPassCutoffFreq, lowPassOrder
        {A7,  0.001, 0.297, 20.0, 0.1, 3},
        {A12, 0.001, 0.297, 20.0, 0.1, 3}
    };

    // Amphenol PM sensor parameters
    // --------------------------------------------------------------------------------------------

    const AmphenolPMSensorParam DefaultAmphenolPMSensorParam = 
    {  // smallParticlePin, largeParticlePin, sensitivity,  sampleWindowDt (ms),
        32, 33, 1.5, 60000
    };

    // Optical particle coutner OPCN2 parameters
    // --------------------------------------------------------------------------------------------
    const OPCN2Param DefaultOPCN2Param = 
    {   // spiClock , spiBitOrder, spiDataMode, spiCsPin
        750000, MSBFIRST, SPI_MODE1, 24                
    };
}

