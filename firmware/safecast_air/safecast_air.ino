#include <Streaming.h>
#include <SPI.h>
#include "gas_sensor.h"
#include "amphenol_pm_sensor.h"
#include "opcn2.h"


void setup()
{
    // this is the magic trick for snprintf to support float
      asm(".global _snprintf_float");

    //GasSensors.initialize();
    //AmphenolPMSensor.initialize();

    //GasSensors.start();
    //AmphenolPMSensor.start();


    Serial.begin(115200);

    ParticleCounterOPCN2.initialize();

    bool status = ParticleCounterOPCN2.checkStatus();
    if (status)
    {
        bool ok;
        ParticleCounterOPCN2.setFanAndLaserOn(&ok);
        Serial << "setFanAndLaserOn ok = " << ok << endl;
    }
}

void loop()
{
    static unsigned long loopCnt = 0;
    OPCN2Data cntrData = ParticleCounterOPCN2.getHistogramData();
    Serial << "PM1:   " << cntrData.PM1   << endl;
    Serial << "PM2.5: " << cntrData.PM2_5 << endl; 
    Serial << "PM10:  " << cntrData.PM10  << endl;
    Serial << endl;

    //String infoString = ParticleCounterOPCN2.getInfoString();
    //Serial << infoString << endl;

    //Serial << endl << "Gas Sensors" << endl;
    //for (auto &sensor : GasSensors)
    //{
    //    if (sensor.isActive())
    //    {
    //        Serial << "  " << sensor.gasName() << ":  " << sensor.ppbLowPass() << endl; 
    //    }
    //}
    //Serial << endl << endl;

    //if (AmphenolPMSensor.haveSample())
    //{
    //    Serial << "PM Sensors" << endl;
    //    Serial << "  (small) pcs/m^3:    " <<  AmphenolPMSensor.countPerCubicFt(SmallParticle) << endl;
    //    Serial << "  (large) pcs/m^3:    " <<  AmphenolPMSensor.countPerCubicFt(LargeParticle) << endl;
    //    Serial << "  (small) pulse Cnt:  " <<  AmphenolPMSensor.pulseCount(SmallParticle) << endl;
    //    Serial << "  (large) pulse Cnt:  " <<  AmphenolPMSensor.pulseCount(LargeParticle) << endl;
    //    Serial << endl << endl;
    //}

    loopCnt++;
    //delay(300);
    delay(15000);
}



