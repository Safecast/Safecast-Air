#include <Streaming.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "opcn2.h"
#include "fixed_vector.h"

//const unsigned long sampleInterval = 60000;
const unsigned long sampleInterval = 10000;

const int OLED_DC = 5;
const int OLED_CS = 3;
const int OLED_RESET = 4;
const int DispBufSize = 50;
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);
SPISettings dispSPISettings(4000000,MSBFIRST,SPI_MODE0);

const OPCN2Param OPCN2Param = 
{   // spiClock , spiBitOrder, spiDataMode, spiCsPin
    750000, MSBFIRST, SPI_MODE1, 24                
};
OPCN2 ParticleCounter(OPCN2Param);

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

    ParticleCounter.initialize();

    bool status = ParticleCounter.checkStatus();
    bool ok;;
    ParticleCounter.setFanAndLaserOn(&ok);

    SPI.beginTransaction(dispSPISettings);
    display.clearDisplay();
    display.setCursor(0,0);
    snprintf(dispBuf, DispBufSize, "st: %d", status);
    display.println(dispBuf);
    display.println();
    snprintf(dispBuf, DispBufSize, "ok: %d", ok);
    display.println(dispBuf);
    display.display();
    SPI.endTransaction();

    delay(1000);
}

void loop()
{
    static int cnt = 0;
    char dispBuf[DispBufSize];

    OPCN2Data cntrData = ParticleCounter.getHistogramData();

    if (cnt > 0)
    {

        // Write data to LCD
        SPI.beginTransaction(dispSPISettings);
        display.clearDisplay();
        display.setCursor(0,0);
        snprintf(dispBuf, DispBufSize, "cnt    %d", cnt);
        display.println(dispBuf);
        display.println();
        snprintf(dispBuf, DispBufSize, "PM1    %3.2f", cntrData.PM1);
        display.println(dispBuf);
        display.println();
        snprintf(dispBuf, DispBufSize, "PM2.5  %3.2f", cntrData.PM2_5);
        display.println(dispBuf);
        display.println();
        snprintf(dispBuf, DispBufSize, "PM10   %3.2f", cntrData.PM10);
        display.println(dispBuf);
        display.println();
        display.display();
        SPI.endTransaction();

        // Write data to openlog
        float time = float(cnt)*(float(sampleInterval)*0.001);
        snprintf(dispBuf, DispBufSize, "%f", time); 
        Serial3.print(dispBuf);
        snprintf(dispBuf, DispBufSize, " %f", cntrData.PM1); 
        Serial3.print(dispBuf);
        snprintf(dispBuf, DispBufSize, " %f", cntrData.PM2_5);
        Serial3.print(dispBuf);
        snprintf(dispBuf, DispBufSize, " %f", cntrData.PM10);
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
}





