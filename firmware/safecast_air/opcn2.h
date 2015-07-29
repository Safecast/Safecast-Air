#ifndef OPCN2_H
#define OPCN2_H
#include "opcn2_param.h"
#include "opcn2_data.h"


class OPCN2
{
    public:

        static const unsigned int StartupDelayMs = 2000;
        static const unsigned int SPICmdDelayMs = 10;
        static const unsigned int SPIValDelayUs = 10;

        static const uint8_t CmdCheckStatus= 0xcf;
        static const uint8_t CmdLaserFanOnOff = 0x03;
        static const uint8_t CmdLaserFanSetPower = 0x42;
        static const uint8_t CmdReadInfoString = 0x3f;
        static const uint8_t CmdReadHistogramData = 0x30;

        static const int InfoMessageSize = 61;        // Number of bytes in SPI response
        static const int InfoStringLength = 26;       // Number of actual values in string
        static const int HistogramMessageSize = 63;   // Number of bytes in SPI response

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

//extern OPCN2 ParticleCounterOPCN2;
//
extern OPCN2 ParticleCounter1;
extern OPCN2 ParticleCounter2;

#endif 
