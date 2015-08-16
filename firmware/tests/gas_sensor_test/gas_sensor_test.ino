#include <Streaming.h>
#include <SPI.h>
#include "fixed_vector.h"
#include "sensor_dev_vector.h"
#include "filter.h"
#include "gas_sensor.h"

const int numGasSensor = 6;

const SamplingParam samplingParam = 
{   // timerPriority,  ainResolution;, ainAveraging,  sampleDt; 
    100, 16, 32, 50 
};

const GasSensorParam sensorParam[numGasSensor] = 
{   // gasType, serialNumber, workingAinPin, auxillaryAinPin, workingZero, auxillaryZero, sensitivity, 
    // ainVRef, ainScaleFact, powerScaleFact, lowPassCutoffFreq, lowPassOrder, active
    
    {GAS_TYPE_NO2, 161740024ul, A0,  A1,  0.234,  0.235,  0.435,  1.2,  4.4,  1.00,  0.1,  3, true}, 
    {GAS_TYPE_O3,  165417321ul, A2,  A3,  0.416,  0.416,  0.321,  1.2,  4.4,  1.00,  0.1,  3, true},
    {GAS_TYPE_CO,  162740011ul, A4,  A5,  0.398,  0.346,  0.403,  1.2,  4.4,  1.00,  0.1,  3, true},
    {GAS_TYPE_NO2, 161740022ul, A6,  A7,  0.226,  0.226,  0.374,  1.2,  4.4,  1.00,  0.1,  3, true},
    {GAS_TYPE_O3,  165417320ul, A8,  A9,  0.405,  0.417,  0.283,  1.2,  4.4,  1.00,  0.1,  3, true},
    {GAS_TYPE_CO,  162740010ul, A10, A11, 0.406,  0.363,  0.381,  1.2,  4.4,  1.00,  0.1,  3, true}
};

GasSensorDevVector<numGasSensor> gasSensors(samplingParam, sensorParam);

void setup()
{
    Serial.begin(115200);

    // Setup gas sensors
    gasSensors.initialize();
    gasSensors.setTimerCallback( []() { gasSensors.sample(); } );
    gasSensors.start();
}

void loop()
{
    Serial << "Gas Sensors" << endl;
    for (auto &sensor : gasSensors)
    {
        if (sensor.isActive())
        {
            Serial << "  name:         " << sensor.gasName() << endl; 
            Serial << "  workingInt:   " << sensor.workingInt() << endl;
            Serial << "  working:      " << sensor.working() << endl;
            Serial << "  auxillaryInt: " << sensor.auxillaryInt() << endl;
            Serial << "  auxillary:    " << sensor.auxillary() << endl;
            Serial << "  ppb:          " << sensor.ppb() << endl;
            Serial << "  ppbLowPass:   " << sensor.ppbLowPass() << endl;
            Serial << endl;
        }
    }
    Serial << "--------------------" << endl << endl; 

    delay(1000);  // Display update period (sensor update is set in sampling params.)
}


