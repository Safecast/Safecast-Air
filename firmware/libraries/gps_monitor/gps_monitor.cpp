#include "gps_monitor.h"

GPSMonitor::GPSMonitor()
{ }

GPSMonitor::GPSMonitor(HardwareSerial *serialPtr)
{
    gpsSerialPtr_ = serialPtr;
    gps_ = Adafruit_GPS(serialPtr);
}

void GPSMonitor::initialize()
{
    timer_.priority(timerPriority_);

    gpsSerialPtr_ -> begin(BaudRate);
    gps_.begin(BaudRate);
    gps_.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    gps_.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   
    gps_.sendCommand(PGCMD_ANTENNA);
}

void GPSMonitor::setTimerCallback(void (*timerCallback)())
{
    timerCallback_ = timerCallback;
}

void GPSMonitor::start()
{
    timer_.begin(timerCallback_, readPeriodUs_);
}


void GPSMonitor::stop()
{
    timer_.end();
}

bool GPSMonitor::haveData() 
{
    return haveData_ || haveNewData();
}

bool GPSMonitor::haveNewData() 
{
    return gps_.newNMEAreceived(); 
}

GPSData GPSMonitor::getData(bool *ok) 
{
    if (ok != nullptr)
    {
        *ok = false;
    }
    if (haveNewData())
    {
        bool newDataOk = false;
        GPSData gpsDataTmp = getNewData(&newDataOk);
        if (newDataOk)
        {
            gpsData_ = gpsDataTmp;
        }
    }
    if (haveData_ && (ok != nullptr))
    {
        *ok = true;
    }
    return gpsData_;
}

GPSData GPSMonitor::getNewData(bool *ok) 
{ 
    if (ok != nullptr)
    {
        *ok = false;
    }
    GPSData data;
    if (gps_.parse(gps_.lastNMEA()))   
    {
        if (ok != nullptr)
        {
            *ok = true;
        }
        haveData_ = true;

        data.fix = gps_.fix; 
        data.fixquality = gps_.fixquality;
        data.satellites = gps_.satellites;
        data.year = gps_.year;
        data.month = gps_.month;
        data.day = gps_.day;
        data.hour = gps_.hour;
        data.minute = gps_.minute;
        data.seconds = gps_.seconds;
        data.milliseconds = gps_.milliseconds;
        data.latitudeFixed = gps_.latitude_fixed;
        data.longitudeFixed = gps_.longitude_fixed;
        data.latitude = gps_.latitude;
        data.longitude = gps_.longitude;
        data.latitudeDegrees = gps_.latitudeDegrees;
        data.longitudeDegrees = gps_.longitudeDegrees;
        data.lat = gps_.lat;
        data.lon = gps_.lon;
        data.mag = gps_.mag;
        data.geoidheight = gps_.geoidheight;
        data.altitude = gps_.altitude;
        data.speed = gps_.speed;
        data.angle = gps_.angle;
        data.magvariation = gps_.magvariation;
        data.HDOP = gps_.HDOP;
        gpsData_ = data;
    }
    return data;
}


void GPSMonitor::update()
{
    if (haveNewData())
    {
        getNewData();
    }
}


void GPSMonitor::reset()
{
    gpsData_ = GPSData();
    haveData_ = false;
}

void GPSMonitor::readData()
{
    gps_.read();
}




