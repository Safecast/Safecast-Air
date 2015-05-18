#include <limits.h>
#include <Streaming.h>
#include <TimerOne.h>
#include <util/atomic.h>

#include "constants.h"
#include "filter.h"
#include "alphasense_gas_sensor.h"
#include "amphenol_pm_sensor.h"




// PM Sensor Test
// --------------------------------------------------------------------------------------


void setup()
{
    Serial.begin(115200);
    PMSensor.initialize();

}

void loop()
{
    static unsigned long cnt = 0;
    Serial << cnt << endl;
    Serial << "small particle occ:  " <<  PMSensor.smallParticleOccupancy() << endl;
    Serial << "small particle cnt:  " <<  PMSensor.smallParticleCount()     << endl;
    Serial << "small particle rate: " <<  PMSensor.smallParticleRate()      << endl;
    Serial << endl;
    Serial << "large particle occ:  " <<  PMSensor.largeParticleOccupancy() << endl;
    Serial << "large particle cnt:  " <<  PMSensor.largeParticleCount()     << endl;
    Serial << "large particle rate: " <<  PMSensor.largeParticleRate()      << endl;
    Serial << "--------------------------------------------" << endl;
    cnt++;
    delay(1000);

}


//// Gas Sensor Test
//// --------------------------------------------------------------------------------------
//alphasense::GasSensor gasSensor[constants::NumGasSensor];
//void setup()
//{
//    Serial.begin(115200);
//
//    alphasense::analogInputSetup();
//    for (int i=0; i<constants::NumGasSensor; i++)
//    {
//        gasSensor[i] = alphasense::GasSensor(constants::DefaultGasSensorParam[i]); 
//    }
//
//}
//
//void loop()
//{
//    for (int i=0; i<constants::NumGasSensor; i++)
//    {
//        gasSensor[i].sample(constants::GasSensorSampleDt);
//    }
//
//    for (int i=0; i<constants::NumGasSensor; i++)
//    {
//        Serial << i << ": ";  
//        Serial << gasSensor[i].ppbLowPass() << endl;
//    }
//    Serial << endl;
//    delay(1000*constants::GasSensorSampleDt);
//}
