#include <limits.h>
#include <Streaming.h>
#include <TimerOne.h>
#include <TimerThree.h>
#include <util/atomic.h>

#include "fixed_vector.h"
#include "constants.h"
#include "filter.h"
#include "utility.h"
#include "lookup_table.h"
#include "occupancy_accum.h"
#include "gas_sensor_param.h"
#include "pm_sensor_param.h"
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

    Serial << "loopCnt: " << loopCnt << endl;

    Serial << endl << "Gas Sensors" << endl;
    for (auto &sensor : GasSensors)
    {
        Serial << sensor.ppbLowPass() << " "; 
    }
    Serial << endl << endl;

    Serial << "PM Sensors" << endl;
    if (PMSensor.haveSample())
    {
        Serial << "(small) pcs/m^3:  " <<  PMSensor.countPerCubicFt(SmallParticle) << endl;
        Serial << "(large) pcs/m^3:  " <<  PMSensor.countPerCubicFt(LargeParticle) << endl;
        Serial << endl;
    }

    Serial << endl;
    loopCnt++;
    delay(1000);
}



