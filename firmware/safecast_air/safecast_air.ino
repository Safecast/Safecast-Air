#include <Streaming.h>
#include <SPI.h>
#include "gas_sensor.h"
#include "amphenol_pm_sensor.h"
#include "opcn2.h"


void setup()
{
    //GasSensors.initialize();
    //AmphenolPMSensor.initialize();

    //GasSensors.start();
    //AmphenolPMSensor.start();


    // Note, startup delays seem to be required or you will only get garbage 
    // from the OPC-N2
    delay(2000);
    Serial.begin(115200);

    ParticleCounterOPCN2.initialize();
    delay(2000);
    interrupts();
    bool status = ParticleCounterOPCN2.checkStatus();
    ParticleCounterOPCN2.setFanAndLaserOn();
}

void loop()
{
    static unsigned long loopCnt = 0;
    //ParticleCounterOPCN2.setFanAndLaserOn();
    //ParticleCounterOPCN2.getInfoString();
    ParticleCounterOPCN2.getHistogram();

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
    delay(30000);
    //delay(1000);
}



