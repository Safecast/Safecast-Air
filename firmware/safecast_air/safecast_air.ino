#include <Streaming.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GPS.h>
#include <ArduinoJson.h>
#include "constants.h"
#include "fixed_vector.h"
#include "sensor_dev_vector.h"
#include "filter.h"
#include "gas_sensor.h"
#include "tmp_sensor.h"
#include "opcn2.h"
#include "gps_monitor.h"
#include "logger.h"


Adafruit_SSD1306 display(
    constants::DisplayDC, 
    constants::DisplayReset, 
    constants::DisplayCS
    );

GPSMonitor gpsMonitor;

GasSensorDevVector<constants::NumGasSensor> gasSensors(
    constants::DefaultGasSensorSamplingParam, 
    constants::DefaultGasSensorParam
    );

TmpSensorDevVector<constants::NumTmpSensor> tmpSensors(
    constants::DefaultTmpSensorSamplingParam,
    constants::DefaultTmpSensorParam
    );

OPCN2 particleCounter(constants::DefaultOPCN2Param, constants::DefaultOPCN2Ids);

Logger dataLogger(constants::DefaultLoggerParam);

void setup()
{

    Serial.begin(constants::USBSerialBaudRate);
    Serial << "Initializing" << endl;

    // This is the magic trick for snprintf to support float
    asm(".global _snprintf_float");

    display.begin(SSD1306_SWITCHCAPVCC);
    SPI.beginTransaction(constants::DisplaySPISettings);
    display.clearDisplay();   
    display.display();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("Initializing ... ");
    display.println();
    display.display();
    SPI.endTransaction();

    // Setup GPS monitor
    gpsMonitor.initialize();
    gpsMonitor.setTimerCallback( []() {gpsMonitor.readData(); });
    gpsMonitor.start();

    SPI.beginTransaction(constants::DisplaySPISettings);
    display.println("  * gps");
    display.display();
    SPI.endTransaction();

    // Setup gas sensors
    gasSensors.initialize();
    gasSensors.setTimerCallback( []() { gasSensors.sample(); } );
    gasSensors.start();

    SPI.beginTransaction(constants::DisplaySPISettings);
    display.println("  * gas sensors");
    display.display();
    SPI.endTransaction();

    // Setup temperature sensors
    tmpSensors.initialize();
    tmpSensors.setTimerCallback( []() { tmpSensors.sample(); } );
    tmpSensors.start();

    SPI.beginTransaction(constants::DisplaySPISettings);
    display.println("  * tmp sensors");
    display.display();
    SPI.endTransaction();
    delay(200);  // Short delay seems to be necessary or opcn2 won't will give an error 
                 // and not work properly.

    // Setup particle counter
    particleCounter.initialize();
    bool status = particleCounter.checkStatus();
    bool laserAndFanOk = false;
    particleCounter.setFanAndLaserOn(&laserAndFanOk);

    SPI.beginTransaction(constants::DisplaySPISettings);
    display.print("  * opcn2: ");
    display.print(status);
    display.print(",");
    display.println(laserAndFanOk);
    display.display();
    SPI.endTransaction();

    // Setup dataLogger
    dataLogger.initialize();
    dataLogger.writeConfiguration();
    dataLogger.setTimerCallback( []() { dataLogger.onTimer(); } );
    dataLogger.start();

    delay(500);
    SPI.beginTransaction(constants::DisplaySPISettings);
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("First reading ... ");
    display.display();
    SPI.endTransaction();
}


void loop()
{
    gpsMonitor.update();
    dataLogger.update();
    delay(constants::LoopDelay);

}





