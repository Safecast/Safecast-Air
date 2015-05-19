#include <limits.h>
#include <functional>
#include <Streaming.h>
#include <TimerOne.h>
#include <util/atomic.h>
#include "fixed_vector.h"
#include "constants.h"
#include "filter.h"
#include "gas_sensor_param.h"
#include "pm_sensor_param.h"
#include "gas_sensor.h"
#include "pm_sensor.h"


// PM Sensor Test
// --------------------------------------------------------------------------------------

//void setup()
//{
//    Serial.begin(115200);
//    PMSensor.initialize();
//
//}
//
//void loop()
//{
//    static unsigned long cnt = 0;
//    Serial << cnt << endl;
//
//    Serial << "small particle occ:  " <<  PMSensor.occupancy(SmallParticle) << endl;
//    Serial << "small particle cnt:  " <<  PMSensor.count(SmallParticle)     << endl;
//    Serial << "small particle rate: " <<  PMSensor.rate(SmallParticle)      << endl;
//    Serial << endl;
//    Serial << "large particle occ:  " <<  PMSensor.occupancy(LargeParticle) << endl;
//    Serial << "large particle cnt:  " <<  PMSensor.count(LargeParticle)     << endl;
//    Serial << "large particle rate: " <<  PMSensor.rate(LargeParticle)      << endl;
//    Serial << endl;
//    Serial << "--------------------------------------------" << endl;
//    cnt++;
//    delay(1000);
//}


// Gas Sensor Test
// --------------------------------------------------------------------------------------

void setup()
{
    Serial.begin(115200);
    GasSensors.initialize();
}

void loop()
{
    for (int i=0; i<GasSensors.size(); i++)
    {
        GasSensors[i].sample(constants::GasSensorSampleDt);
    }

    for (int i=0; i<constants::NumGasSensor; i++)
    {
        Serial << i << ": ";  
        Serial << GasSensors[i].ppbLowPass() << endl;
    }

    Serial << endl;
    delay(1000*constants::GasSensorSampleDt);
}
