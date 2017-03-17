#ifndef GPS_DATA_H
#define GPS_DATA_H
#include <Arduino.h>

class GPSData
{ 

    public:

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

        int32_t latitudeFixed;
        int32_t longitudeFixed;

        float latitude;
        float longitude;

        float latitudeDegrees;
        float longitudeDegrees;

        char lat;
        char lon;
        char mag;

        float geoidheight;
        float altitude;

        float speed;
        float angle;
        float magvariation;
        float HDOP;

        GPSData();

        String getDateTimeString();
        String getLatitudeString(bool addNS=false);
        String getLongitudeString(bool addEW=false);
        double getLatitudeDecDeg();
        double getLongitudeDecDeg();
        float getAltitudeInMeter();
        float getSpeedInMeterPerSec();

    protected:

        static const int ScratchArraySize = 200;
        static constexpr float CentimeterToMeter = 1.0e-2;
        static constexpr float KnotsToMeterPerSec = 0.514444;
        static double convertToDecDeg(double valueNEMAish, char hemi);

};

#endif 
