#include <Streaming.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "gas_sensor.h"
#include "opcn2.h"

const int OLED_DC = 5;
const int OLED_CS = 3;
const int OLED_RESET = 4;
const int DispBufSize = 50;
const unsigned long sampleInterval = 60000;

Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

SPISettings dispSPISettings(4000000,MSBFIRST,SPI_MODE0);

void setup()
{

    char dispBuf[DispBufSize];

    // this is the magic trick for snprintf to support float
    asm(".global _snprintf_float");

    Serial.begin(115200);

    // Setup display
    display.begin(SSD1306_SWITCHCAPVCC);
    display.clearDisplay();   
    display.display();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    // Setup openlog
    Serial3.begin(9600);

    // Display Initialization message
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Initializing");
    display.println();
    display.display();

    ParticleCounter1.initialize();
    ParticleCounter2.initialize();

    bool status1 = ParticleCounter1.checkStatus();
    bool status2 = ParticleCounter2.checkStatus();
    bool ok1, ok2;
    ParticleCounter1.setFanAndLaserOn(&ok1);
    ParticleCounter2.setFanAndLaserOn(&ok2);

    SPI.beginTransaction(dispSPISettings);
    display.clearDisplay();
    display.setCursor(0,0);
    snprintf(dispBuf, DispBufSize, "st: %d %d", status1, status2);
    display.println(dispBuf);
    display.println();
    snprintf(dispBuf, DispBufSize, "ok: %d %d", ok1, ok2);
    display.println(dispBuf);
    display.display();
    SPI.endTransaction();

    delay(1000);


    //GasSensors.initialize();
    //AmphenolPMSensor.initialize();

    //GasSensors.start();
    //AmphenolPMSensor.start();
}

void loop()
{
    static int cnt = 0;
    char dispBuf[DispBufSize];

    OPCN2Data cntrData1 = ParticleCounter1.getHistogramData();
    OPCN2Data cntrData2 = ParticleCounter2.getHistogramData();

    if (cnt > 0)
    {

        //Serial << "PM1:   " << cntrData1.PM1   << ", " << cntrData2.PM1   << endl;
        //Serial << "PM2.5: " << cntrData1.PM2_5 << ", " << cntrData2.PM2_5 << endl; 
        //Serial << "PM10:  " << cntrData1.PM10  << ", " << cntrData2.PM10  << endl;
        //Serial << endl;

        // Write data to LCD
        SPI.beginTransaction(dispSPISettings);
        display.clearDisplay();
        display.setCursor(0,0);
        snprintf(dispBuf, DispBufSize, "cnt    %d", cnt);
        display.println(dispBuf);
        display.println();
        snprintf(dispBuf, DispBufSize, "PM1    %3.2f    %3.2f", cntrData1.PM1, cntrData2.PM1);
        display.println(dispBuf);
        display.println();
        snprintf(dispBuf, DispBufSize, "PM2.5  %3.2f    %3.2f", cntrData1.PM2_5, cntrData2.PM2_5);
        display.println(dispBuf);
        display.println();
        snprintf(dispBuf, DispBufSize, "PM10   %3.2f    %3.2f", cntrData1.PM10, cntrData2.PM10);
        display.println(dispBuf);
        display.println();
        display.display();
        SPI.endTransaction();

        // Write data to openlog
        float time = float(cnt)*(float(sampleInterval)*0.001);
        snprintf(dispBuf, DispBufSize, "%f", time); 
        Serial3.print(dispBuf);
        snprintf(dispBuf, DispBufSize, " %f %f", cntrData1.PM1, cntrData2.PM1); 
        Serial3.print(dispBuf);
        snprintf(dispBuf, DispBufSize, " %f %f", cntrData1.PM2_5, cntrData2.PM2_5);
        Serial3.print(dispBuf);
        snprintf(dispBuf, DispBufSize, " %f %f", cntrData1.PM10, cntrData2.PM10);
        Serial3.print(dispBuf);
        Serial3.println();
    }
    else
    {
        SPI.beginTransaction(dispSPISettings);
        display.clearDisplay();
        display.setCursor(0,0);
        display.println("First reading ...");
        display.display();
        SPI.endTransaction();
    }



    delay(sampleInterval);
    cnt++;


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
    //delay(300);
}





