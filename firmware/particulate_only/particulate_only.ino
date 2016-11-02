/*
   A Safecast device for Airquality measurement.

   Copyright (c) 2015, Will Dickson

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

History:

Contact:
   
*/

#include <SPI.h>
#include <i2c_t3.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GPS.h>
#include <Adafruit_SHT31.h>
#include <ArduinoJson.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <EEPROM.h>
#include "constants.h"
#include "configuration.h"
#include "fixed_vector.h"
#include "opcn2.h"
#include "gps_monitor.h"
#include "openlog.h"
#include "logger.h"


Adafruit_SSD1306 display( constants::DisplayDC, constants::DisplayReset, constants::DisplayCS);
Openlog openlog(constants::DefaultOpenlogParam);
Configuration configuration(constants::ConfigurationFile,openlog);
GPSMonitor gpsMonitor;
OPCN2 particleCounter(constants::DefaultOPCN2Param, constants::DefaultOPCN2Ids);
Adafruit_SHT31 humidityAndTempSensor = Adafruit_SHT31();
Logger dataLogger(constants::DefaultLoggerParam,openlog);


void setupDisplay();
void loadConfiguration(); 
void showStartupScreen(); 
void setupGPSMonitor();
void setupParticleCounter();
void setupSHT31();
void setupDataLogger();
void showFirstReadingScreen();


void setup()
{
    // Turn off wifly
    dataLogger.initializeWiFlyPwrPin();

    // This is the magic trick for snprintf to support float
    asm(".global _snprintf_float");

    // USB Serial for debugging, etc.
    Serial.begin(constants::USBSerialBaudRate);

    setupDisplay();
    openlog.initialize();
    loadConfiguration();
    showStartupScreen();
    setupGPSMonitor();
    setupParticleCounter();
    setupSHT31();
    setupDataLogger();
    showFirstReadingScreen();
}


void loop()
{
    Alarm.delay(constants::LoopDelay);
    gpsMonitor.update();
    dataLogger.update();
}


void setupDisplay()
{
    // Required or conflict with OLED display
    pinMode(constants::DefaultOPCN2Param.spiCsPin,OUTPUT);
    digitalWrite(constants::DefaultOPCN2Param.spiCsPin,HIGH);

    display.begin(SSD1306_SWITCHCAPVCC);
    SPI.beginTransaction(constants::DisplaySPISettings);
    display.clearDisplay();   
    display.display();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    SPI.endTransaction();
}


void loadConfiguration()
{
    // Try to load configuration file
    bool ok = false;
    for (int i=0; (i<constants::NumLoadConfigTrys && !ok); i++)
    {
        SPI.beginTransaction(constants::DisplaySPISettings);
        display.clearDisplay();   
        display.setCursor(0,0);
        display.print("load config: ");
        display.println(i);
        display.display();
        SPI.endTransaction();

        ok = configuration.initialize();
        delay(10);
    }

    if (!ok)
    {
        // Failed to load configuration from SD card.
        SPI.beginTransaction(constants::DisplaySPISettings);
        display.clearDisplay();   
        display.setCursor(0,0);
        display.println("error: config file");
        display.println(configuration.errorMsg());
        display.display();
        SPI.endTransaction();
        while (true) {}  // Freeze here
    }
    else
    {
        SPI.beginTransaction(constants::DisplaySPISettings);
        display.clearDisplay();   
        display.setCursor(0,0);
        display.println("config file OK");
        display.display();
        SPI.endTransaction();
    }
    delay(1000);
}


void showStartupScreen()
{
    SPI.beginTransaction(constants::DisplaySPISettings);
    display.clearDisplay();   
    display.setCursor(0,0);
    display.print("Safecast Air V");
    display.println((constants::SoftwareVersion));
    display.print("Device Name=");
    display.println((constants::DeviceName));
    display.print("Device ID=");
    display.println(configuration.deviceId());
    display.display();
    SPI.endTransaction();
    delay(3000);
    SPI.beginTransaction(constants::DisplaySPISettings);
    display.clearDisplay();   
    display.setCursor(0,0);
    display.println("Initializing ... ");
    display.display();
    SPI.endTransaction();
}


void setupGPSMonitor()
{
    // Setup GPS monitor
    gpsMonitor.initialize();
    gpsMonitor.setTimerCallback( []() {gpsMonitor.readData(); });
    gpsMonitor.start();

    SPI.beginTransaction(constants::DisplaySPISettings);
    display.println("* gps");
    display.display();
    SPI.endTransaction();
}


void setupParticleCounter()
{
    // Setup particle counter
    particleCounter.initialize();
    particleCounter.setIds(configuration.opcn2Ids());
    bool status = particleCounter.checkStatus();
    bool laserAndFanOk = false;
    particleCounter.setFanAndLaserOn(&laserAndFanOk);

    SPI.beginTransaction(constants::DisplaySPISettings);
    display.print("* opcn2: ");
    display.print(status);
    display.print(",");
    display.println(laserAndFanOk);
    display.display();
    SPI.endTransaction();

}


void setupSHT31()
{
    // Setup humidity and temperature sensor
    bool status = humidityAndTempSensor.begin(constants::SHT31Address);
    SPI.beginTransaction(constants::DisplaySPISettings);
    display.print("* sht31: ");
    display.println(status);
    display.display();
    SPI.endTransaction();
}


void setupDataLogger()
{
    // Setup dataLogger timer
    bool loggerOk =  dataLogger.initializeFile();
    SPI.beginTransaction(constants::DisplaySPISettings);
    display.print("* log: ");
    display.println(loggerOk);
    display.display();
    SPI.endTransaction();
    delay(2000);
    Alarm.timerRepeat(dataLogger.logWritePeriod(), [](){dataLogger.writeLogOnTimer();});
    Alarm.timerRepeat(dataLogger.dataSamplePeriod(), [](){dataLogger.dataSampleOnTimer();} );

    // After starting timers take reading from particle sensor to clear out old histogram counts
    particleCounter.getHistogramData(); 

    // Setup wifly
    dataLogger.initializeWiFly();

    SPI.beginTransaction(constants::DisplaySPISettings);
    if (dataLogger.haveWiFly())
    {
        if (dataLogger.haveNetwork())
        {
            display.print("* ip: ");
            display.println(dataLogger.ip());
        }
        else
        {
            display.println("* no network");
        }
    }
    else
    {
        display.println("* no wifly");
    }
    display.display();
    SPI.endTransaction();
    delay(2000);
}


void showFirstReadingScreen()
{
    SPI.beginTransaction(constants::DisplaySPISettings);
    display.clearDisplay();
    display.setCursor(0,0);
    display.println();
    display.println("First reading  ... ");
    display.print("takes ");
    display.print(dataLogger.dataSamplePeriod());
    display.println(" seconds");
    display.display();
    SPI.endTransaction();
}
