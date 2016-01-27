#include "mq4_methane.h"
#include <Streaming.h>

const float MilliSecToSec = 1.0e-3;
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);

const MQ4_MethaneParam UndefinedMethaneParam = 
{ // priority, sampleDt, ainPin, ainReadRes, ainReadAvg, ainMaxVal, lowPassCutoffFreq, lowPassOrder,  numLookup, tableVoltToPPM[][2], {ids}
    100, 50, A17, 16, 32, 1.2, 0.1, 2, 5, {{0.0, 0.0}, {0.3, 0.8}, {0.6, 3.6}, {0.9,8.1}, {1.2, 14.4}}, {0,1,2}
};

MQ4_Methane::MQ4_Methane()
{
    setParam(UndefinedMethaneParam);
}

MQ4_Methane::MQ4_Methane(MQ4_MethaneParam param)
{
    setParam(param);
}

void MQ4_Methane::initialize()
{
    // Note, could conflict with gas sensor initialization
    analogReadRes(param_.ainReadRes);
    analogReadAveraging(param_.ainReadAvg);
    analogReference(INTERNAL);

    ain_ = 0.0;
    ppm_ = 0.0;
    ppmLowPassFilter_.reset();
}


void MQ4_Methane::setParam(MQ4_MethaneParam param)
{
    param_ = param;
    ppmLowPassFilter_ = filter::LowPass(param_.lowPassCutoffFreq, param_.lowPassOrder, 0.0);
}

void MQ4_Methane::setTimerCallback(void (*timerCallback)())
{
    timerCallback_ = timerCallback;
}


MQ4_MethaneParam MQ4_Methane::getParam() const
{
    return param_;
}


String MQ4_Methane::type()
{
    return String("MQ4-CH4");
}


float MQ4_Methane::ain() const
{
    return ain_;
}


float MQ4_Methane::ppm() const
{
    return ppm_;
}


float MQ4_Methane::ppmLowPass() const
{
    return ppmLowPassFilter_.value();
}

void MQ4_Methane::start()
{ 
    initialize(); 
    timer_.begin(timerCallback_,  getSampleDtUs());
}

void MQ4_Methane::stop()
{
    timer_.end();
}

void MQ4_Methane::sample()
{
    uint16_t rawADC = analogRead(A17);
    ain_ = param_.ainMaxVal*float(rawADC)/float(UINT16_MAX); 
    ppm_ = ppmFromVolt(ain_);
    float dt = param_.sampleDt;
    ppmLowPassFilter_.update(ppm_, MilliSecToSec*dt);
}


float MQ4_Methane::ppmFromVolt(float volt)
{
    float ppm = 0.0;
    if (volt < param_.tableVoltToPPM[0][0])
    {
        ppm =  param_.tableVoltToPPM[0][1];
    }
    else if (volt > param_.tableVoltToPPM[param_.numLookup-1][0])
    {
        ppm = param_.tableVoltToPPM[param_.numLookup-1][1];
    }
    else
    {
        for (int i=1; i<param_.numLookup; i++)
        {
            if (volt <= param_.tableVoltToPPM[i][0])
            {
                float logppm = mapfloat( 
                    log(volt), 
                    log(param_.tableVoltToPPM[i-1][0]), 
                    log(param_.tableVoltToPPM[i][0]), 
                    log(param_.tableVoltToPPM[i-1][1]), 
                    log(param_.tableVoltToPPM[i][1])
                    );
                ppm = exp(logppm);
                break;
            }
        }
    }
    return ppm;
}

unsigned long MQ4_Methane::getSampleDtUs()
{
    return 1000ul*param_.sampleDt;
};


float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
     return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
