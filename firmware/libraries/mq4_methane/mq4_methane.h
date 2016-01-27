#ifndef MQ4_METHANE_H
#define MQ4_METHANE_H
#include <Arduino.h>
#include "mq4_methane_param.h"
#include "filter.h"

class MQ4_Methane
{
    public:
        MQ4_Methane();
        MQ4_Methane(MQ4_MethaneParam param);
        void initialize();

        void setParam(MQ4_MethaneParam param);
        MQ4_MethaneParam getParam() const;
        String type();
        float ain() const;
        float ppm() const;
        float ppmLowPass() const;
        void sample();
        void start();
        void stop();
        void setTimerCallback(void (*timerCallback)());

    protected:

        MQ4_MethaneParam param_;
        IntervalTimer timer_;
        float ain_;
        float ppm_;
        filter::LowPass ppmLowPassFilter_;
        float ppmFromVolt(float volt);
        unsigned long getSampleDtUs();

        static void dummyTimerCallback() { }; 
        void (*timerCallback_)() = dummyTimerCallback; 
};


#endif
