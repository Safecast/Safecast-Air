#include <Streaming.h>
#include "fixed_vector.h"
#include "sensor_dev_vector.h"
#include "filter.h"
#include "tmp_sensor.h"

const int numTmpSensor = 2;

const SamplingParam samplingParam = 
{
    100, 16, 32, 50
};

const TmpSensorParam sensorParam[NumTmpSensor] = 
{
    {A13, 0.5, 10.0, 1.2, 1.0, 0.1, 3, true},
    {A12, 0.5, 10.0, 1.2, 1.0, 0.1, 3, true}
};


TmpSensorDevVector<cnumTmpSensor> tmpSensors(samplingParam, sensorParam);

void setup()
{
    Serial.begin(115200);

    // Setup temperature sensors
    tmpSensors.initialize();
    tmpSensors.setTimerCallback( []() { tmpSensors.sample(); } );
    tmpSensors.start();
}

void loop()
{
    Serial << endl << "Tmp Sensors" << endl;
    for (auto &sensor : tmpSensors)
    {
        if (sensor.isActive())
        {
            Serial << "  val: " << sensor.value()  << "  lowPass: " << sensor.valueLowPass() << endl;
        }
    }
    Serial << "--------------------" << endl << endl; 

    delay(1000);
}




