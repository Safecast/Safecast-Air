#ifndef OPCN2_DATA_H
#define OPCN2_DATA_H
#include "Arduino.h"

class OPCN2Data
{

    public:
        static const int NumHistogramBins = 16;
        static const int ScratchArraySize = 50;

        uint16_t binCount[NumHistogramBins];
        uint8_t bin1_mtof;
        uint8_t bin3_mtof;
        uint8_t bin5_mtof;
        uint8_t bin7_mtof;
        float flowRate;
        uint32_t temperature;
        bool haveTemperature;
        uint32_t pressure;
        bool havePressure;
        float  samplePeriod;
        uint16_t checksum;
        float PM1;
        float PM2_5;
        float PM10;

        OPCN2Data();
        OPCN2Data(uint8_t spiData[]);
        void fromSPIData(uint8_t spiData[]);
        String toString();
};

#endif
