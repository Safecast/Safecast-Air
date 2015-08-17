/******************************************************************************
    Notes: needed to edit Adafruit_GPS.cpp to get to work - changed isDigit
    and isAlpha to isdigit and isalpha. 
 ******************************************************************************/
#include <Adafruit_GPS.h>
#include <Streaming.h>

#define gpsSerial Serial1
const unsigned long gpsReadDt = 1000;
void gpsReadCallback();

Adafruit_GPS GPS(&gpsSerial);
IntervalTimer timer;
volatile bool haveNewData = false;


void setup()   
{                
    Serial.begin(9600);

    // Setup GPS
    GPS.begin(9600);
    gpsSerial.begin(9600);

    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
    GPS.sendCommand(PGCMD_ANTENNA);

    timer.priority(115);
    timer.begin(gpsReadCallback,gpsReadDt);
}


void loop() 
{
    static unsigned long lastUpdate = 0;

    if (GPS.newNMEAreceived()) {
        if (!GPS.parse(GPS.lastNMEA()))   
            return;  
    }

    noInterrupts();
    if (millis() - lastUpdate > 1000)
    {
        lastUpdate = millis();

        Serial << "GPS Data" << endl;
        Serial << "----------------------------------------------" << endl;
        Serial << _DEC(GPS.day)     << "/";
        Serial << _DEC(GPS.month)   << "/20"; 
        Serial << _DEC(GPS.year)    << " ";
        Serial << _DEC(GPS.hour)    << ":";
        Serial << _DEC(GPS.minute)  << ":";
        Serial << _DEC(GPS.seconds) << endl;
        Serial << "fix: "  << int(GPS.fix) << " ";
        Serial << "qual: " << int(GPS.fixquality) << " ";
        Serial << "num: "  << int(GPS.satellites);
        if (GPS.fix)
        {
            Serial << endl;
            Serial << "  lat:   " << _FLOAT(GPS.latitude,4)  <<  GPS.lat << endl;
            Serial << "  lon:   " << _FLOAT(GPS.longitude,4) <<  GPS.lon << endl;
            Serial << "  konts: " << GPS.speed << endl;
            Serial << "  angle: " << GPS.angle << endl;
            Serial << "  alt:   " << GPS.angle << endl;
        }
        Serial << endl;
        haveNewData = false;
    }
    interrupts();

    delay(500);
}


void gpsReadCallback()
{
    GPS.read();
}
