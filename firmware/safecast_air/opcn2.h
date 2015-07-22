#ifndef OPCN2_H
#define OPCN2_H
#include "opcn2_param.h"

class OPCN2
{
    public:
        OPCN2(OPCN2Param param);
        void initialize();
        bool checkStatus();
        void setFanAndLaserOn();
        void setFanAndLaserOff();
        void getInfoString();
        void getHistogram();

    protected:

        OPCN2Param param_;
        SPISettings spiSettings_;
};

extern OPCN2 ParticleCounterOPCN2;

#endif 
