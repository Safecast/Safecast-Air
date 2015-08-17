#ifndef GPS_MONITOR_H
#define GPS_MONITOR_H
#include "gps_data.h"
#include <Adafruit_GPS.h>
#include <Arduino.h>


class GPSMonitor
{
    public:

        static const unsigned long DefaultReadPeriodUs = 1000;
        static const uint8_t DefaultTimerPriority = 120;

        GPSMonitor();
        void initialize();
        bool dataReady();
        GPSData getData() const;

    protected:

        uint8_t timerPriority_ = DefaultTimerPriority;
        unsigned long readPeriodUs_ = DefaultReadPeriodUs;
        IntervalTimer timer_;
        Adafruit_GPS gps_ = Adafruit_GPS(&Serial1);

};


#endif

