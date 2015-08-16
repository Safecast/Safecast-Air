/*********************************************************************

Notes: needed to edit Adafruit_GPS.cpp to get to work - changed isDigit
and isAlpha to isdigit and isalpha. 

 *********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GPS.h>
#include <TimerOne.h>

#define gpsSerial Serial1
Adafruit_GPS GPS(&gpsSerial);

// Uncomment this block to use hardware SPI
#define OLED_DC     5
#define OLED_CS     3
#define OLED_RESET  4
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int readCnt = 0;
const unsigned long updateDt = 2000;

void gpsReadCallback();

void setup()   
{                
    Serial.begin(9600);

    // Set up display
    display.begin(SSD1306_SWITCHCAPVCC);
    display.clearDisplay();   
    display.display();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    // Setup GPS
    GPS.begin(9600);
    gpsSerial.begin(9600);

    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
    GPS.sendCommand(PGCMD_ANTENNA);

    Timer1.initialize(1000);
    Timer1.attachInterrupt(gpsReadCallback);
    Timer1.start();
}


void loop() 
{
    static unsigned long lastUpdateT = 0;

    if (GPS.newNMEAreceived()) {
        if (!GPS.parse(GPS.lastNMEA()))   
            return;  
    }

    if ((millis() - lastUpdateT) > updateDt)
    {
        lastUpdateT = millis();

        display.clearDisplay();
        display.setCursor(0,0);
        display.println("GPS Test");

        display.print(GPS.day, DEC); 
        display.print('/');
        display.print(GPS.month, DEC); 
        display.print("/20");
        display.print(GPS.year, DEC);
        display.print(" ");
        display.print(GPS.hour, DEC); 
        display.print(':');
        display.print(GPS.minute, DEC); 
        display.print(':');
        display.println(GPS.seconds, DEC); 

        display.print("fix: "); 
        display.print((int)GPS.fix);
        display.print(" qual: "); 
        display.print((int)GPS.fixquality); 
        display.print(" num: "); 
        display.print((int)GPS.satellites);
        if (GPS.fix) 
        {
            display.print(GPS.latitude, 4); 
            display.println(GPS.lat);
            display.print(GPS.longitude, 4); 
            display.println(GPS.lon);
            display.print("knots: "); 
            display.println(GPS.speed);
            display.print("angle: "); 
            display.println(GPS.angle);
            display.print("altitude: "); 
            display.println(GPS.altitude);
        }
    }

    display.display();
    delay(100);
}


void gpsReadCallback()
{
    readCnt++;
    GPS.read();
}
