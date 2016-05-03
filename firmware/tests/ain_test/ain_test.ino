#include <SPI.h>
#include "fixed_vector.h"
#include "sensor_dev_vector.h"
#include "filter.h"
#include "gas_sensor.h"

const int NumGasSensor = 3;
const int BufSize = 200;

const SamplingParam samplingParam = 
{   // timerPriority,  ainResolution;, ainAveraging,  sampleDt; 
    100, 16, 32, 50 
};

const GasSensorParam sensorParam[NumGasSensor] = 
{   // gasType, serialNumber, header, position, workingAinPin, auxillaryAinPin, workingZero, auxillaryZero,
    // sensitivity, ainVRef, ainScaleFact, powerScaleFact, lowPassCutoffFreq, lowPassOrder, active, {ids}
    {GAS_TYPE_NO2, 131410831ul, 0, 0, A0,  A1,  0.292,  0.319,  0.357,  1.2,  4.4,  1.00,  0.005,  3, true, {54,  55, 56,  57}}, 
    {GAS_TYPE_O3,  135410519ul, 0, 1, A2,  A3,  0.410,  0.415,  0.183,  1.2,  4.4,  1.00,  0.005,  3, true, {58,  59, 60,  61}},
    {GAS_TYPE_CO,  132410527ul, 0, 2, A4,  A5,  0.326,  0.270,  0.468,  1.2,  4.4,  1.00,  0.005,  3, true, {62,  63, 64,  65}}
};


GasSensorDevVector<NumGasSensor> gasSensors(samplingParam, sensorParam);

void setup()
{
    asm(".global _snprintf_float");

    Serial.begin(115200);

    // Setup gas sensors
    gasSensors.initialize();
    gasSensors.setTimerCallback( []() { gasSensors.sample(); } );
    gasSensors.start();
}

void loop()
{
    char buf[BufSize];

    for (auto &sensor : gasSensors)
    {
        if (sensor.isActive())
        {
            Serial.print("name:         "); 
            Serial.println(sensor.gasName());; 

            Serial.print("working:      "); 
            //Serial.println(sensor.working());
            snprintf(buf,BufSize,"%1.5f",sensor.working()); 
            Serial.println(buf);

            Serial.print("auxillary:    "); 
            //Serial.println(sensor.auxillary());
            snprintf(buf,BufSize,"%1.5f",sensor.auxillary()); 
            Serial.println(buf);

            Serial.println();
        }
    }
    Serial.println("--------------------");
    Serial.println();

    delay(1000);  // Display update perio);d (sensor update is set in sampling params.)
}


