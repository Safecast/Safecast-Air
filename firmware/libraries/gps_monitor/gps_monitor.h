#ifndef GPS_MONITOR_H
#define GPS_MONITOR_H
#include "gps_data.h"
#include <Adafruit_GPS.h>
#include <Arduino.h>


class GPSMonitor
{
    public:

        static const unsigned long BaudRate = 9600;
        static const unsigned long DefaultReadPeriodUs = 1000;
        static const uint8_t DefaultTimerPriority = 12;

        GPSMonitor();
        GPSMonitor(HardwareSerial *serialPtr);
        void initialize();
        void start();
        void stop();
        void setTimerCallback(void (*timerCallback)());

        bool haveData();
        bool haveNewData();

        GPSData getData(bool *ok=nullptr);
        GPSData getNewData(bool *ok=nullptr);

        void update();
        void reset();
        void readData();

    protected:

        uint8_t timerPriority_ = DefaultTimerPriority;
        unsigned long readPeriodUs_ = DefaultReadPeriodUs;
        IntervalTimer timer_;

        static void dummyTimerCallback() { }; 
        void (*timerCallback_)() = dummyTimerCallback; 

        HardwareSerial *gpsSerialPtr_ = &Serial1;
        Adafruit_GPS gps_ = Adafruit_GPS(gpsSerialPtr_);

        bool haveData_ = false;
        GPSData gpsData_;

};


#endif

