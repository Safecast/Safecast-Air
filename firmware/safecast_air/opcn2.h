#ifndef OPCN2_H
#define OPCN2_H
#include "opcn2_param.h"
#include "opcn2_data.h"


class OPCN2
{
    public:

        static const unsigned int STARTUP_DELAY_MS = 2000;
        static const unsigned int SPI_CMD_DELAY_MS = 10;
        static const unsigned int SPI_VAL_DELAY_US = 10;

        static const uint8_t CMD_CHECK_STATUS = 0xcf;
        static const uint8_t CMD_LASER_FAN_ON_OFF = 0x03;
        static const uint8_t CMD_LASER_FAN_SET_POWER = 0x42;
        static const uint8_t CMD_READ_INFO_STRING = 0x3f;
        static const uint8_t CMD_READ_HISTOGRAM_DATA = 0x30;

        static const int INFO_MESSAGE_SIZE = 61;        // Number of bytes in SPI response
        static const int INFO_STRING_LENGTH = 26;       // Number of actual values in string
        static const int HISTOGRAM_MESSAGE_SIZE = 63;   // Number of bytes in SPI response

        OPCN2(OPCN2Param param);
        void initialize();
        bool checkStatus();
        void setFanAndLaserOn(bool *ok=nullptr);
        void setFanAndLaserOff(bool *ok=nullptr);
        String getInfoString(bool *ok=nullptr);
        OPCN2Data getHistogramData(bool *ok=nullptr);

    protected:

        OPCN2Param param_;
        SPISettings spiSettings_;
};

extern OPCN2 ParticleCounterOPCN2;

#endif 
