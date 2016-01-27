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

#include <Streaming.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GPS.h>
#include <ArduinoJson.h>
#include "constants.h"
#include "filter.h"
#include "gps_monitor.h"
#include "logger.h"
#include "mq4_methane.h"


Adafruit_SSD1306 display(
    constants::DisplayDC, 
    constants::DisplayReset, 
    constants::DisplayCS
    );

GPSMonitor gpsMonitor;

Logger dataLogger(constants::DefaultLoggerParam);

MQ4_Methane methaneSensor(constants::DefaultMethaneParam);

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
    display.println();
    display.print("SafecastAir V");
    display.println((constants::SoftwareVersion));
    display.println();
    display.print("DeviceName=");
    display.println((constants::DeviceName));
    display.println();
    display.print("DeviceID=");
    display.println((constants::DeviceId));
    display.display();
    SPI.endTransaction();
    delay(1500);

    // Setup GPS monitor
    gpsMonitor.initialize();
    gpsMonitor.setTimerCallback( []() {gpsMonitor.readData(); });
    gpsMonitor.start();

    SPI.beginTransaction(constants::DisplaySPISettings);
    display.clearDisplay();   
    display.setCursor(0,0);
    display.println("Initializing ... ");
    display.println();
    display.println("  * gps");
    display.display();
    SPI.endTransaction();
    delay(500);

    // Setup Methane Sensor
    methaneSensor.initialize();
    methaneSensor.setTimerCallback( []() {methaneSensor.sample();});
    methaneSensor.start();

    SPI.beginTransaction(constants::DisplaySPISettings);
    display.println("  * methane");
    display.display();
    SPI.endTransaction();
    delay(500);

    // Setup dataLogger
    dataLogger.initialize();
    dataLogger.writeConfiguration();
    dataLogger.setTimerCallback( []() { dataLogger.onTimer(); } );
    dataLogger.start();

    SPI.beginTransaction(constants::DisplaySPISettings);
    display.println("  * logger");
    display.display();
    SPI.endTransaction();

    delay(1000);
}


void loop()
{
    gpsMonitor.update();
    dataLogger.update();
    delay(constants::LoopDelay);

    GPSData gpsData; 
    bool gpsDataOk = false;
    if (gpsMonitor.haveData())
    {
        gpsData = gpsMonitor.getData(&gpsDataOk);
    }
    if (gpsDataOk)
    {
        String dateTimeString = gpsData.getDateTimeString();
        String latitudeString = gpsData.getLatitudeString();
        String longitudeString = gpsData.getLongitudeString();

        SPI.beginTransaction(constants::DisplaySPISettings);
        display.clearDisplay();
        display.setCursor(0,0);
        display.println(dateTimeString.c_str());
        display.println();
        display.print("SAT:  ");
        display.println(gpsData.satellites);
        display.print("LAT:  ");
        display.println(latitudeString.c_str());
        display.print("LON:  ");
        display.println(longitudeString.c_str());
        display.print("ALT:  ");
        display.print(gpsData.getAltitudeInMeter());
        display.println("(m)"); 
        display.print("SPD:  ");
        display.print(gpsData.getSpeedInMeterPerSec());
        display.println("(m/s)");
        display.print("CH4:  ");
        display.print(methaneSensor.ppmLowPass());
        display.println("(ppm) ");
        display.display();
        SPI.endTransaction();
    }
}





