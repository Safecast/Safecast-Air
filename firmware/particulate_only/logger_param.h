#ifndef LOGGER_PARAM_H
#define LOGGER_PARAM_H
#include <Arduino.h>

struct LoggerParam
{
   unsigned long logWritePeriod;
   unsigned long dataSamplePeriod;
   unsigned long baudRate;
};

#endif
