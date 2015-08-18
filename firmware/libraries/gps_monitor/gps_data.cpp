#include "gps_data.h"

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
    snprintf(scratch,ScratchArraySize,"20%d-%d-%dT%d:%d:%dZ",year,month,day,hour,minute,seconds);
    return String(scratch);
}


String GPSData::getLatitudeString(bool addNS)
{
    char scratch[ScratchArraySize];
    if (addNS)
    {
        snprintf(scratch,ScratchArraySize,"%09.4f%c",latitude,lat);
    }
    else
    {
        snprintf(scratch,ScratchArraySize,"%09.4f",latitude);
    }
    return String(scratch);
}


String GPSData::getLongitudeString(bool addEW)
{
    char scratch[ScratchArraySize];
    if (addEW)
    {
        snprintf(scratch,ScratchArraySize,"%010.4f%c",longitude,lon);
    }
    else
    {
        snprintf(scratch,ScratchArraySize,"%010.4f",longitude);
    }
    return String(scratch);
}
