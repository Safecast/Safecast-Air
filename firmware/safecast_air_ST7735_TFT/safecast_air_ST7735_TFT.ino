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

History: Init
2016-11-6 Init

Contact: rob@yr-design.biz

   
*/

#include <Streaming.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX_AS.h>    // Core graphics library
#include <Adafruit_ST7735_AS.h> // Hardware-specific library

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

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


#define OLED_CLK 13
#define OLED_DATA 11
#define OLED_DC 5
#define OLED_CS 3
#define OLED_RESET 4


Adafruit_ST7735_AS display = Adafruit_ST7735_AS(OLED_CS, OLED_DC, OLED_RESET);       // Invoke custom library
// Adafruit_ST7735 display = Adafruit_ST7735(OLED_CS, OLED_DC, OLED_RESET);

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
    
    // Required or conflict with OLED display ... maybe add OPCN2 ... some kind of initialization
    pinMode(constants::DefaultOPCN2Param.spiCsPin,OUTPUT);
    digitalWrite(constants::DefaultOPCN2Param.spiCsPin,HIGH);
    
    Serial.begin(constants::USBSerialBaudRate);
    Serial << "Initializing" << endl;
    Serial2.begin(9600);

    // This is the magic trick for snprintf to support float
    asm(".global _snprintf_float");

//    display.begin(SSD1306_SWITCHCAPVCC);
//  SPI.beginTransaction(constants::DisplaySPISettings);
//    display.clearDisplay();   
//    display.display();
    display.initR(INITR_BLACKTAB); //
    display.setRotation(2);
    display.fillScreen(ST7735_BLACK);
    display.setTextSize(1);
//    display.setTextColor(WHITE);
    display.print("Safecast Air V");
    display.println((constants::SoftwareVersion));
    display.print("Device Name=");
    display.println((constants::DeviceName));
    display.print("Device ID=");
    display.println((constants::DeviceId));
//    display.display();
    display.println("Initializing ... ");
    display.println();
//    display.display();
    SPI.endTransaction();

    // Setup GPS monitor
    gpsMonitor.initialize();
    gpsMonitor.setTimerCallback( []() {gpsMonitor.readData(); });
    gpsMonitor.start();

    SPI.beginTransaction(constants::DisplaySPISettings);
    display.println("  * gps");
//    display.display();
    SPI.endTransaction();

    // Setup gas sensors
    gasSensors.initialize();
    gasSensors.setTimerCallback( []() { gasSensors.sample(); } );
    gasSensors.start();

    SPI.beginTransaction(constants::DisplaySPISettings);
    display.println("  * gas sensors");
//    display.display();
    SPI.endTransaction();

    // Setup temperature sensors
    tmpSensors.initialize();
    tmpSensors.setTimerCallback( []() { tmpSensors.sample(); } );
    tmpSensors.start();

    SPI.beginTransaction(constants::DisplaySPISettings);
    display.println("  * tmp sensors");
//    display.display();
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
//    display.display();
    SPI.endTransaction();

    // Setup dataLogger
    dataLogger.initialize();
    dataLogger.writeConfiguration();
    dataLogger.setTimerCallback( []() { dataLogger.onTimer(); } );
    dataLogger.start();

    delay(500);
//    SPI.beginTransaction(constants::DisplaySPISettings);
//    display.clearDisplay();
    display.fillScreen(ST7735_BLACK);
    display.setCursor(0,0);
    display.println("First reading  ... ");
    display.println("takes 60 seconds");
//    display.display();
//    SPI.endTransaction();
}


void loop()
{
    gpsMonitor.update();
    dataLogger.update();
    delay(constants::LoopDelay);

}





