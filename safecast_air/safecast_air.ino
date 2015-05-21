#include <Streaming.h>
#include "gas_sensor.h"
#include "pm_sensor.h"


void setup()
{
    GasSensors.initialize();
    PMSensor.initialize();

    GasSensors.start();
    PMSensor.start();

    Serial.begin(115200);
}

void loop()
{
    static unsigned long loopCnt = 0;

    Serial << "loop count: " << loopCnt << endl;

    Serial << endl << "Gas Sensors" << endl;
    for (auto &sensor : GasSensors)
    {
        if (sensor.isActive())
        {
            Serial << "  " << sensor.gasName() << ":  " << sensor.ppbLowPass() << endl; 
        }
    }
    Serial << endl << endl;

    if (PMSensor.haveSample())
    {
        Serial << "PM Sensors" << endl;
        Serial << "  (small) pcs/m^3:    " <<  PMSensor.countPerCubicFt(SmallParticle) << endl;
        Serial << "  (large) pcs/m^3:    " <<  PMSensor.countPerCubicFt(LargeParticle) << endl;
        Serial << "  (small) pulse Cnt:  " <<  PMSensor.pulseCount(SmallParticle) << endl;
        Serial << "  (large) pulse Cnt:  " <<  PMSensor.pulseCount(LargeParticle) << endl;
        Serial << endl << endl;
    }

    loopCnt++;
    delay(1000);
}



