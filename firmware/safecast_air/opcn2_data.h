#ifndef OPCN2_DATA_H
#define OPCN2_DATA_h
#include "Arduino.h"

class OPCN2Data
{
    static const int NUM_HISTOGRAM_BINS = 16;

    public:
        uint16_t binCount[NUM_HISTOGRAM_BINS];
        uint8_t bin1_mtof;
        uint8_t bin3_mtof;
        uint8_t bin5_mtof;
        uint8_t bin7_mtof;
        float flowRate;
        uint32_t temperature;
        uint32_t pressure;
        uint32_t samplingPeriod;
        uint16_t checksum;
        float PM1;
        float PM2_5;
        float PM10;

        OPCN2Data();
        OPCN2Data(uint8_t spiData[]);
        void fromSPIData(uint8_t spiData[]);
};

#endif