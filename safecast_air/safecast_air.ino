#include "constants.h"
#include "alphasense_gas_sensor.h"
#include "Streaming.h"


alphasense::GasSensor gas_sensor[constants::num_gas_sensor];

void setup()
{
    Serial.begin(115200);
    alphasense::analogInputSetup();
    for (auto i=0; i<constants::num_gas_sensor; i++)
    {
        gas_sensor[i] = alphasense::GasSensor(constants::gas_sensor_param[i]); 
    }

}

void loop()
{
    // Test read from sensors
    for (auto i=0; i<constants::num_gas_sensor; i++)
    {
        gas_sensor[i].sample(constants::gas_sensor_sample_dt);
    }

    for (auto i=0; i<constants::num_gas_sensor; i++)
    {
        Serial << i << ": ";  
        Serial << gas_sensor[i].getPPBFilt() << endl;
    }
    Serial << endl;


    delay(1000*constants::gas_sensor_sample_dt);
}
