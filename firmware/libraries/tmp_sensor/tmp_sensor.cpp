#include "tmp_sensor.h"
#include <Streaming.h>
#include <cstdint>

const int   AinMaxInt   = UINT16_MAX;
const float MilliVoltPerVolt = 1000.0;
const float MilliSecPerSec = 1.0e-3;

const TmpSensorParam UndefinedSensorParam = 
    {
        A0,       // ainPin
        0.0,      // offset
        1.0,      // sensitivity
        1.2,      // ainVRef
        1.0,      // ainScaleFact
        1.0,      // lowPassCutoffFreq
        1,        // lowPassOrder
        false    // active
    };

TmpSensorDev::TmpSensorDev() 
{
    setParam(UndefinedSensorParam);
    initialize();
} 


TmpSensorDev::TmpSensorDev(TmpSensorParam param)
{
    setParam(param);
    initialize();
}

void TmpSensorDev::sample(unsigned long dt)
{
    rawInt_ = analogRead(param_.ainPin);
    rawVolt_ = (param_.ainVRef*float(rawInt_)/float(AinMaxInt))*param_.ainScaleFact; 
    value_ = MilliVoltPerVolt*(rawVolt_ - param_.offset)/param_.sensitivity;
    lowPassFilter_.update(value_, MilliSecPerSec*dt);
}

TmpSensorParam TmpSensorDev::param() const
{
    return param_;
}


void TmpSensorDev::setParam(TmpSensorParam param)
{
    param_ = param;
    lowPassFilter_ = filter::LowPass(param_.lowPassCutoffFreq, param_.lowPassOrder, 0.0);
}

bool TmpSensorDev::isActive() const
{
    return param_.active;
}


void TmpSensorDev::initialize()
{
    value_ = 0.0;
    lowPassFilter_.reset();
}

float TmpSensorDev::value() const
{
    return value_;
}

float TmpSensorDev::valueLowPass() const
{
    return lowPassFilter_.value();
}

int TmpSensorDev::rawInt() const
{
    return rawInt_;
}

float TmpSensorDev::rawVolt() const
{
    return rawVolt_;
}
