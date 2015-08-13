#include <Streaming.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "fixed_vector.h"
#include "sensor_dev_vector.h"
#include "filter.h"
#include "gas_sensor.h"
#include "tmp_sensor.h"
#include "opcn2.h"
#include "constants.h"

//const int OLED_DC = 5;
//const int OLED_CS = 3;
//const int OLED_RESET = 4;
//const int DispBufSize = 50;
//const unsigned long sampleInterval = 60000;
//
//
//Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);
//SPISettings dispSPISettings(4000000,MSBFIRST,SPI_MODE0);

GasSensorDevVector<constants::NumGasSensor> gasSensors(
    constants::GasSensorSamplingParam, 
    constants::DefaultGasSensorParam
    );


TmpSensorDevVector<constants::NumTmpSensor> tmpSensors(
    constants::TmpSensorSamplingParam,
    constants::DefaultTmpSensorParam
    );

void setup()
{
    // This is the magic trick for snprintf to support float
    asm(".global _snprintf_float");

    Serial.begin(115200);

    gasSensors.initialize();
    tmpSensors.initialize();

    gasSensors.setTimerCallback( []() { gasSensors.sample(); } );
    tmpSensors.setTimerCallback( []() { tmpSensors.sample(); } );

    gasSensors.start();
    tmpSensors.start();
}

void loop()
{
    Serial << "Gas Sensors" << endl;
    for (auto &sensor : gasSensors)
    {
        if (sensor.isActive())
        {
            Serial << "  name:         " << sensor.gasName() << endl; 
            Serial << "  workingInt:   " << sensor.workingInt() << endl;
            Serial << "  working:      " << sensor.working() << endl;
            Serial << "  auxillaryInt: " << sensor.auxillaryInt() << endl;
            Serial << "  auxillary:    " << sensor.auxillary() << endl;
            Serial << "  ppb:          " << sensor.ppb() << endl;
            Serial << "  ppbLowPass:   " << sensor.ppbLowPass() << endl;
            Serial << endl;

            //Serial << "  name: " << sensor.gasName(); 
            //Serial << "  " << sensor.ppb();
            //Serial << ", " << sensor.ppbLowPass() << endl;
        }
    }
    Serial << endl << "Tmp Sensors" << endl;
    for (auto &sensor : tmpSensors)
    {
        if (sensor.isActive())
        {
            Serial << "  " << sensor.value()  << "  " << sensor.valueLowPass() << endl;
        }
    }
    Serial << "--------------------" << endl;
    Serial << endl; 

    delay(1000);
}




