#include "logger.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GPS.h>
#include <Adafruit_SHT31.h>
#include "constants.h"
#include "opcn2_ids.h"

extern GPSMonitor gpsMonitor;
extern OPCN2 particleCounter;
extern Adafruit_SSD1306 display;
extern Adafruit_SHT31 humidityAndTempSensor; 

Logger::Logger(LoggerParam param, HardwareSerial *serialPtr)
{
    param_ = param;
    serialPtr_ = serialPtr;
}


void Logger::initialize()
{
    serialPtr_ -> begin(param_.baudRate);
}

unsigned long Logger::period()
{
    return param_.period;
}

void Logger::onTimer()
{
    writeDataFlag_ = true;
}

void Logger::update()
{
    Serial.print("writeDataFlag_ = ");
    Serial.println(writeDataFlag_);
    if (writeDataFlag_)
    {
        writeData();
        writeDisplay();
        writeDataFlag_ = false;
    }
}

void Logger::writeConfiguration()
{
    // Not done ... not being used ... 
    
    jsonBuffer_ = StaticJsonBuffer<JsonBufferSize>(); // Clear the buffer 
    JsonObject &rootObj = jsonBuffer_.createObject();
    rootObj["dev"] = constants::DeviceName;  // Change to something from loaded settings
    rootObj["id"] = constants::DeviceId;     // Change to something from loaded settings
    rootObj["msg"] = "cfg";

}


void Logger::writeData()
{
    // Get particle counter data
    opcn2Data_ = particleCounter.getHistogramData();
    OPCN2Ids opcn2Ids = particleCounter.getIds();

    // Get GPS position data
    bool gpsDataOk = false;
    if (gpsMonitor.haveData())
    {
        gpsData_ = gpsMonitor.getData(&gpsDataOk);
    }
    if (!gpsDataOk)
    {
        return;
    }
    String dateTimeString = gpsData_.getDateTimeString();
    String latitudeString = gpsData_.getLatitudeString();
    String longitudeString = gpsData_.getLongitudeString();

    // Get humidity and temperature
    humidity_ = humidityAndTempSensor.readHumidity(); 
    temperature_ =  humidityAndTempSensor.readTemperature();

    // Create Json output data
    jsonBuffer_ = StaticJsonBuffer<JsonBufferSize>(); // Clear the buffer 
    JsonObject &rootObj = jsonBuffer_.createObject();
    rootObj["dev"] = constants::DeviceName;           // Change to something from loaded settings
    rootObj["id"] = constants::DeviceId;              // Change to something from loaded settings
    rootObj["msg"] = "dat";

    // Add GPS data
    JsonObject &gpsObj = rootObj.createNestedObject("gps");
    gpsObj["date"] = dateTimeString.c_str();
    gpsObj["lat"] = latitudeString.c_str();
    gpsObj["lon"] = longitudeString.c_str();
    gpsObj["alt"] = gpsData_.getAltitudeInMeter();
    gpsObj["spd"] = gpsData_.getSpeedInMeterPerSec();
    gpsObj["ang"] = gpsData_.angle;
    gpsObj["fix"] = gpsData_.fix;
    gpsObj["num"] = gpsData_.satellites;

    // Add OPN2 sensor  
    {
        JsonArray &pmArray = rootObj.createNestedArray("pm");
        JsonObject &opcn2Obj = pmArray.createNestedObject();
        opcn2Obj["pos"] = "norm";
        opcn2Obj["pm1"] = opcn2Data_.PM1;
        opcn2Obj["pm2_5"] = opcn2Data_.PM2_5;
        opcn2Obj["pm10"] = opcn2Data_.PM10;
        opcn2Obj["rate"] = opcn2Data_.flowRate;
        opcn2Obj["dt"] = opcn2Data_.samplePeriod;
        opcn2Obj["mtof1"] = opcn2Data_.bin1_mtof;
        opcn2Obj["mtof3"] = opcn2Data_.bin3_mtof;
        opcn2Obj["mtof5"] = opcn2Data_.bin5_mtof;
        opcn2Obj["mtof7"] = opcn2Data_.bin7_mtof;
        JsonArray &binCntArray = opcn2Obj.createNestedArray("cnt");
        for (int i=0; i<OPCN2Data::NumHistogramBins; i++)
        {
            binCntArray.add(opcn2Data_.binCount[i]);
        }

        JsonObject &idsObj = opcn2Obj.createNestedObject("ids");
        idsObj["pm1"] = opcn2Ids.pm1;
        idsObj["pm2_5"] = opcn2Ids.pm2_5;
        idsObj["pm10"] = opcn2Ids.pm10;
        idsObj["rate"] = opcn2Ids.rate;
        idsObj["dt"] = opcn2Ids.dt;
        idsObj["mtof1"] = opcn2Ids.mtof1;
        idsObj["mtof3"] = opcn2Ids.mtof3;
        idsObj["mtof5"] = opcn2Ids.mtof5;
        idsObj["mtof7"] = opcn2Ids.mtof7;
        JsonArray &cntIdsArray = idsObj.createNestedArray("cnt");
        for (int i=0; i<OPCN2Data::NumHistogramBins; i++)
        {
            cntIdsArray.add(opcn2Ids.cnt[i]);
        }
    }

    // Write json object to open log
    rootObj.printTo(*serialPtr_);
    serialPtr_ -> println();
}


void Logger::writeDisplay() // Temporary hack - create separate display function at some point.
{
    // Display the data
    const int col2 = 50;
    const int col3 = 90;
    char tmpStr[50];

    SPI.beginTransaction(constants::DisplaySPISettings);            
    display.clearDisplay();
    display.setCursor(0,0);

    display.print("PM1");
    display.setCursor(col2,display.getCursorY());
    display.print(opcn2Data_.PM1);
    display.setCursor(col3,display.getCursorY());
    display.println("ug/m3");

    display.print("PM2.5");
    display.setCursor(col2,display.getCursorY());
    display.print(opcn2Data_.PM2_5);
    display.setCursor(col3,display.getCursorY());
    display.println("ug/m3");

    display.print("PM10");
    display.setCursor(col2,display.getCursorY());
    display.print(opcn2Data_.PM10);
    display.setCursor(col3,display.getCursorY());
    display.println("ug/m3");

    display.print("TEMP");
    display.setCursor(col2,display.getCursorY());
    snprintf(tmpStr,sizeof(tmpStr),"%1.2f",temperature_);
    display.print(tmpStr);
    display.setCursor(col3,display.getCursorY());
    display.println("C");

    display.print("HUMD");
    display.setCursor(col2,display.getCursorY());
    snprintf(tmpStr,sizeof(tmpStr), "%1.2f",humidity_);
    display.print(tmpStr);
    display.setCursor(col3,display.getCursorY());
    display.println("%");

    display.display();

    SPI.endTransaction();
}
