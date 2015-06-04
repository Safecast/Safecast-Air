#include "pm_sensor.h"
#include "constants.h"
#include <Streaming.h>

// PMSensorDev public methods
// --------------------------------------------------------------------------------------------

const PMSensorParam UndefinedSensorParam = {32, 33, 1.5, 2000};

volatile bool PMSensorDev::haveSample_ = false;
volatile unsigned long PMSensorDev::overflowCnt_ = 0;

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


PMSensorParam PMSensorDev::param() const
{
    return param_;
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

    timer_.priority(TimerPriority);
    reset();
}


void PMSensorDev::start()
{
    reset();
    timer_.begin(PMSensorDev::onTimerOverflow,TimerPeriodUs);
}


void PMSensorDev::stop()
{
    timer_.end();
}


void PMSensorDev::reset()
{
    stop();
    haveSample_ = false;
    overflowCnt_= 0;
    resetParticleAccum(SmallParticle);
    resetParticleAccum(LargeParticle);
}


float PMSensorDev::occupancy(ParticleType particleType) const
{
    return particleAccum_[particleType].value();
}


unsigned long PMSensorDev::pulseCount(ParticleType particleType) const
{
    return particleAccum_[particleType].count();
}


float PMSensorDev::pulseRate(ParticleType particleType) const
{
    return  float(particleAccum_[particleType].count())/float(param_.sampleWindowDt);
}


void PMSensorDev::updateParticleAccum(ParticleType particleType)
{
    particleAccum_[particleType].onPinChange();
}


void PMSensorDev::resetParticleAccum(ParticleType particleType)
{
    particleAccum_[particleType].onResetTimer();
}


unsigned long PMSensorDev::getSampleWindowDt() const
{
    return param_.sampleWindowDt;
}

float PMSensorDev::countPerCubicFt(ParticleType particleType) const
{
    float slope = (100000.0/param_.sensitivity);
    return 100*slope*occupancy(particleType);
}


bool PMSensorDev::haveSample() 
{
    return haveSample_;
}

// PMSensorDev Protected methods
// --------------------------------------------------------------------------------------------

void PMSensorDev::onTimerOverflow()
{
    unsigned long  elapsedTime = overflowCnt_*(PMSensorDev::TimerPeriodUs/1000);
    if (elapsedTime >= PMSensor.getSampleWindowDt())
    {
        haveSample_ = true;
        overflowCnt_ = 0;
        PMSensor.resetParticleAccum(LargeParticle);
        PMSensor.resetParticleAccum(SmallParticle);
    }
    else
    {
        overflowCnt_++;
    }
}


PMSensorDev PMSensor(constants::DefaultPMSensorParam);



