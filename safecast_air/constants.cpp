#include "constants.h" 
#include <Arduino.h>


namespace constants
{

    // Gas Sensor parameters
    // --------------------------------------------------------------------------------------------
    const int GasSensorAinResolution = 15;
    const int GasSensorAinAveraging = 32;
    const unsigned long GasSensorSampleDt = 20; // ms

    const GasSensorParam DefaultGasSensorParam[NumGasSensor] = 
    {
        // gasType, workingAinPin, auxillaryAinPin, workingZero, auxillaryZero, sensitivity, 
        // ainScaleFact, powerScaleFact, lowPassCutoffFreq, lowPassOrder, temperatureSensor
        {GAS_TYPE_NO2, A0,  A1,   0.282,  0.296,  0.305,  4.4,  1.2,  0.05,  3, 0}, 
        {GAS_TYPE_O3,  A2,  A3,   0.422,  0.407,  0.235,  4.4,  1.2,  0.05,  3, 0},
        {GAS_TYPE_CO,  A4,  A5,   0.272,  0.250,  0.497,  4.4,  1.2,  0.05,  3, 0},
        {GAS_TYPE_NO2, A6,  A7,   0.292,  0.319,  0.357,  4.4,  1.2,  0.05,  3, 1},
        {GAS_TYPE_O3,  A8,  A9,   0.410,  0.415,  0.183,  4.4,  1.2,  0.05,  3, 1},
        {GAS_TYPE_CO,  A10, A11,  0.326,  0.270,  0.468,  4.4,  1.2,  0.05,  3, 1}
    };


    // PM sensor parameters
    // --------------------------------------------------------------------------------------------


    // smallParticlePin, largeParticlePin, sensitivity,  sampleWindow,
    const PMSensorParam DefaultPMSensorParam = {32, 33, 1.5, 60000};

}

