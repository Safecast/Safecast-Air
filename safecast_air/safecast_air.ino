#include "constants.h"
#include "alphasense_gas_sensor.h"
#include "filter.h"

#include "Streaming.h"


alphasense::GasSensor gasSensor[constants::NumGasSensor];

void setup()
{
    Serial.begin(115200);
    alphasense::analogInputSetup();
    for (int i=0; i<constants::NumGasSensor; i++)
    {
        gasSensor[i] = alphasense::GasSensor(constants::GasSensorParam[i]); 
    }

}

void loop()
{
    // Test read from sensors
    for (int i=0; i<constants::NumGasSensor; i++)
    {
        gasSensor[i].sample(constants::GasSensorSampleDt);
    }

    for (int i=0; i<constants::NumGasSensor; i++)
    {
        Serial << i << ": ";  
        Serial << gasSensor[i].ppbLowPass() << endl;
    }
    Serial << endl;


    delay(1000*constants::GasSensorSampleDt);
}
