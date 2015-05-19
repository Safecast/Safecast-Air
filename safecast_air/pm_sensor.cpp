#include "pm_sensor.h"
#include "constants.h"
#include "Streaming.h"

#include "Arduino.h"
#include <functional>
#include <TimerOne.h>



// PMSensorDev public methods
// --------------------------------------------------------------------------------------------

const PMSensorParam UndefinedSensorParam = {32, 33, 1.5, 2000};

PMSensorDev::PMSensorDev()
{ 
    setParam(UndefinedSensorParam);
}

PMSensorDev::PMSensorDev(PMSensorParam param) 
{
    setParam(param);
}

void PMSensorDev::setParam(PMSensorParam param)
{
    param_ = param;
}

PMSensorParam PMSensorDev::param()
{
    return param_;
}


float PMSensorDev::occupancy(ParticleType particleType)
{
    return particleAccum_[particleType].value();
}


unsigned long PMSensorDev::count(ParticleType particleType)
{
    return particleAccum_[particleType].count();
}


float PMSensorDev::rate(ParticleType particleType)
{
    return  float(particleAccum_[particleType].count())/float(param_.sampleWindowDt);
}

void PMSensorDev::initialize()
{
    particleAccum_[SmallParticle].initialize(param_.smallParticlePin);
    attachInterrupt(
            param_.smallParticlePin, 
            PMSensorDev::onPinChange<SmallParticle>, 
            CHANGE
            );

    particleAccum_[LargeParticle].initialize(param_.largeParticlePin);
    attachInterrupt(
            param_.largeParticlePin, 
            PMSensorDev::onPinChange<LargeParticle>, 
            CHANGE
            );

    Timer1.initialize();
    Timer1.setPeriod(TimerPeriodUs);
    Timer1.attachInterrupt(PMSensorDev::onTimerOverFlow);
}

void PMSensorDev::updateParticleAccum(ParticleType particleType)
{
    particleAccum_[particleType].onPinChange();
}

void PMSensorDev::resetParticleAccum(ParticleType particleType)
{
    particleAccum_[particleType].onResetTimer();
}

unsigned int PMSensorDev::getSampleWindowDt()
{
    return param_.sampleWindowDt;
}

// PMSensorDev Protected methods
// --------------------------------------------------------------------------------------------

void PMSensorDev::onTimerOverFlow()
{
    static unsigned long cnt = 0;
    unsigned long  elapsedTime = cnt*(PMSensorDev::TimerPeriodUs/1000);
    if (elapsedTime >= PMSensor.getSampleWindowDt())
    {
        cnt = 0;
        PMSensor.resetParticleAccum(LargeParticle);
        PMSensor.resetParticleAccum(SmallParticle);
    }
    else
    {
        cnt++;
    }
}


PMSensorDev PMSensor(constants::DefaultPMSensorParam);



