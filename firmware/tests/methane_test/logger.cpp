#include "logger.h"
#include <Streaming.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GPS.h>
#include <WiFlyHQ.h>
#include "constants.h"
#include "gps_monitor.h"
#include "mq4_methane.h"

extern GPSMonitor gpsMonitor;
extern Adafruit_SSD1306 display;
extern MQ4_Methane methaneSensor;
extern rnxv::WiFly wifly;

Logger::Logger(LoggerParam param, HardwareSerial *serialPtr)
{
    param_ = param;
    serialPtr_ = serialPtr;
}


void Logger::initialize()
{
    serialPtr_ -> begin(param_.baudRate);
}


void Logger::start()
{
    timer_.priority(param_.priority);
    timer_.begin(timerCallback_, param_.period);
}


void Logger::stop()
{
    timer_.end();
}

void Logger::onTimer()
{
    writeDataFlag_ = true;
}


void Logger::update()
{
    if (writeDataFlag_)
    {
        writeData();
        writeDataFlag_ = false;
    }
}


void Logger::setTimerCallback(void (*timerCallback)())
{
    timerCallback_ = timerCallback;
}

void Logger::writeConfiguration()
{
    jsonBuffer_ = StaticJsonBuffer<JsonBufferSize>(); // Clear the buffer 
    JsonObject &rootObj = jsonBuffer_.createObject();
    rootObj["dev"] = constants::DeviceName;  // Change to something from loaded settings
    rootObj["id"] = constants::DeviceId;     // Change to something from loaded settings
    rootObj["msg"] = "cfg";

    // Add gas sensor metadata
    //JsonArray &gasArray = rootObj.createNestedArray("gas");
    // Dev
    // ----------------------------------------
    //rootObj.prettyPrintTo(Serial);
    //Serial << endl;
    // ----------------------------------------
}


void Logger::writeData()
{
    GPSData gpsData; 
    bool gpsDataOk = false;
    if (gpsMonitor.haveData())
    {
        gpsData = gpsMonitor.getData(&gpsDataOk);
    }
    if (!gpsDataOk)
    {
        return;
    }

    String dateTimeString = gpsData.getDateTimeString();
    String latitudeString = gpsData.getLatitudeString();
    String longitudeString = gpsData.getLongitudeString();

    // Create Json output data
    jsonBuffer_ = StaticJsonBuffer<JsonBufferSize>(); // Clear the buffer 
    JsonObject &rootObj = jsonBuffer_.createObject();
    rootObj["dev"] = constants::DeviceName;           // Change to something from loaded settings
    rootObj["id"] = constants::DeviceId;              // Change to something from loaded settings
    rootObj["ver"] = constants::SoftwareVersion;
    rootObj["msg"] = "dat";

    // Add GPS data
    JsonObject &gpsObj = rootObj.createNestedObject("gps");
    gpsObj["date"] = dateTimeString.c_str();
    gpsObj["lat"] = latitudeString.c_str();
    gpsObj["lon"] = longitudeString.c_str();
    gpsObj["alt"] = gpsData.getAltitudeInMeter();
    gpsObj["spd"] = gpsData.getSpeedInMeterPerSec();
    gpsObj["ang"] = gpsData.angle;
    gpsObj["fix"] = gpsData.fix;
    gpsObj["num"] = gpsData.satellites;
    gpsObj["ang"] = 1.4;

    // Add UEXT array
    JsonArray &uextArray = rootObj.createNestedArray("uext");
    JsonObject &sensorObj = uextArray.createNestedObject();
    sensorObj["type"] = methaneSensor.type().c_str();
    sensorObj["ain"] = methaneSensor.ain();
    sensorObj["ppm"] = methaneSensor.ppm();
    sensorObj["ppmFlt"] = methaneSensor.ppmLowPass();

    JsonObject &idsObj = sensorObj.createNestedObject("ids");
    MQ4_MethaneParam param = methaneSensor.getParam();
    idsObj["ain'"] = param.ids.ain;
    idsObj["ppm'"] = param.ids.ppm;
    idsObj["ppmFlt'"] = param.ids.ppmFlt;

    // Write json object to open log
    rootObj.printTo(*serialPtr_);
    *serialPtr_ << endl;

    count_++;

    // DEV - write json Object to wifi
    // --------------------------------------------------
    char buf[JsonBufferSize];
    rootObj.printTo(buf,JsonBufferSize);
    String paramsLength = String(strlen(buf));

    wifly.open("192.168.1.105", 5000);
    wifly.println("POST /jsontest HTTP/1.1");
    wifly.println("Host: 192.168.1.105:5000");
    wifly.println("Content-type: application/json");
    wifly.println("Accept: application/json");
    wifly.print("Content-Length: ");
    wifly.println(paramsLength);
    wifly.println("User-Agent: easyPEP/0.0.1");
    wifly.println();
    wifly.println(buf);
    wifly.close();

    //rootObj.printTo(Serial);
    //Serial << endl;
    //Serial << buf << endl;


}



