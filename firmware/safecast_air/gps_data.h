#ifndef GPS_DATA_H
#define GPS_DATA_H
#include <Arduino.h>

struct GPSData
{ 
    bool fix;
    uint8_t fixquality;
    uint8_t satellites;

    uint8_t year;
    uint8_t month;
    uint8_t day;

    uint8_t hour;
    uint8_t minute;
    uint8_t seconds;
    uint16_t milliseconds;

    int32_t latitude_fixed;
    int32_t longitude_fixed;

    float latitude;
    float longitude;

    float latitudeDegrees;
    float longitudeDegrees;

    char lat;
    char lon;
    char mag;

    float speed;
    float angle;
    float magvariation;


};

#endif 
