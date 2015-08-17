#include "gps_monitor.h"

GPSMonitor::GPSMonitor()
{ }

void GPSMonitor::initialize()
{
    timer_.priority(timerPriority_);
}


GPSData GPSMonitor::getData() const 
{ 
    // Not done
    GPSData data;
    return data;
}


