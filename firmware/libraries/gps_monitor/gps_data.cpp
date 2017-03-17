#include "gps_data.h"
#include <cmath>

GPSData::GPSData()
{
        fix = 0;
        fixquality = 0;
        satellites = 0;
        year = 0;
        month = 0;
        day = 0;
        hour = 0;
        minute = 0;
        seconds = 0;
        milliseconds = 0;
        latitudeFixed = 0;
        longitudeFixed = 0;
        latitude = 0.0;
        longitude = 0.0;
        latitudeDegrees = 0.0;
        longitudeDegrees = 0.0;
        lat = 'X';
        lon = 'X';
        mag = 'X';
        geoidheight = 0.0;
        altitude = 0.0;
        speed = 0.0;
        angle = 0.0;
        magvariation = 0.0;
        HDOP = 0.0;
}


String GPSData::getDateTimeString()
{
    char scratch[ScratchArraySize];
    snprintf(scratch,ScratchArraySize,"20%d-%02d-%02dT%02d:%02d:%02dZ",year,month,day,hour,minute,seconds);
    return String(scratch);
}


String GPSData::getLatitudeString(bool addNS)
{
    char scratch[ScratchArraySize];
    double latitudeDecDeg = getLatitudeDecDeg();
    if (addNS)
    {
        snprintf(scratch,ScratchArraySize,"%.8f%c",latitudeDecDeg,lat);
    }
    else
    {
        snprintf(scratch,ScratchArraySize,"%.8f",latitudeDecDeg);
    }
    return String(scratch);
}


String GPSData::getLongitudeString(bool addEW)
{
    char scratch[ScratchArraySize];
    double longitudeDecDeg = getLongitudeDecDeg();
    if (addEW)
    {
        snprintf(scratch,ScratchArraySize,"%.8f%c",longitudeDecDeg,lon);
    }
    else
    {
        snprintf(scratch,ScratchArraySize,"%.8f",longitudeDecDeg);
    }
    return String(scratch);
}


double GPSData::getLatitudeDecDeg()
{
    return convertToDecDeg(latitude,lat);
}


double GPSData::getLongitudeDecDeg()
{
    return convertToDecDeg(longitude,lon);
}


float GPSData::getAltitudeInMeter()
{
    return altitude*CentimeterToMeter;
}


float GPSData::getSpeedInMeterPerSec()
{
    return speed*KnotsToMeterPerSec;
}

double GPSData::convertToDecDeg(double valueNEMAish,char hemi)
{
    double decDeg = double(trunc(valueNEMAish/100.0));
    double decMinute = double(valueNEMAish) - 100.0*decDeg;
    decDeg += decMinute/60.0;
    if ((hemi == 'S') || (hemi == 'W'))
    {
        decDeg = -decDeg;
    }
    return decDeg;
}
