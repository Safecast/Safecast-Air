#include "amphenol_pm_sensor.h"
#include "constants.h"

// AmphenolPMSensorDev public methods
// --------------------------------------------------------------------------------------------

const AmphenolPMSensorParam UndefinedSensorParam = {32, 33, 1.5, 2000};

volatile bool AmphenolPMSensorDev::haveSample_ = false;
volatile unsigned long AmphenolPMSensorDev::overflowCnt_ = 0;

AmphenolPMSensorDev::AmphenolPMSensorDev()
{ 
    setParam(UndefinedSensorParam);
}


AmphenolPMSensorDev::AmphenolPMSensorDev(AmphenolPMSensorParam param) 
{
    setParam(param);
}


void AmphenolPMSensorDev::setParam(AmphenolPMSensorParam param)
{
    param_ = param;
}


AmphenolPMSensorParam AmphenolPMSensorDev::param() const
{
    return param_;
}


void AmphenolPMSensorDev::initialize()
{
    particleAccum_[SmallParticle].initialize(param_.smallParticlePin);
    attachInterrupt(
            param_.smallParticlePin, 
            AmphenolPMSensorDev::onPinChange<SmallParticle>, 
            CHANGE
            );

    particleAccum_[LargeParticle].initialize(param_.largeParticlePin);
    attachInterrupt(
            param_.largeParticlePin, 
            AmphenolPMSensorDev::onPinChange<LargeParticle>, 
            CHANGE
            );

    timer_.priority(TimerPriority);
    reset();
}


void AmphenolPMSensorDev::start()
{
    reset();
    timer_.begin(AmphenolPMSensorDev::onTimerOverflow,TimerPeriodUs);
}


void AmphenolPMSensorDev::stop()
{
    timer_.end();
}


void AmphenolPMSensorDev::reset()
{
    stop();
    haveSample_ = false;
    overflowCnt_= 0;
    resetParticleAccum(SmallParticle);
    resetParticleAccum(LargeParticle);
}


float AmphenolPMSensorDev::occupancy(ParticleType particleType) const
{
    return particleAccum_[particleType].value();
}


unsigned long AmphenolPMSensorDev::pulseCount(ParticleType particleType) const
{
    return particleAccum_[particleType].count();
}


float AmphenolPMSensorDev::pulseRate(ParticleType particleType) const
{
    return  float(particleAccum_[particleType].count())/float(param_.sampleWindowDt);
}


void AmphenolPMSensorDev::updateParticleAccum(ParticleType particleType)
{
    particleAccum_[particleType].onPinChange();
}


void AmphenolPMSensorDev::resetParticleAccum(ParticleType particleType)
{
    particleAccum_[particleType].onResetTimer();
}


unsigned long AmphenolPMSensorDev::getSampleWindowDt() const
{
    return param_.sampleWindowDt;
}

float AmphenolPMSensorDev::countPerCubicFt(ParticleType particleType) const
{
    float slope = (100000.0/param_.sensitivity);
    return 100*slope*occupancy(particleType);
}


bool AmphenolPMSensorDev::haveSample() 
{
    return haveSample_;
}

// AmphenolPMSensorDev Protected methods
// --------------------------------------------------------------------------------------------

void AmphenolPMSensorDev::onTimerOverflow()
{
    unsigned long  elapsedTime = overflowCnt_*(AmphenolPMSensorDev::TimerPeriodUs/1000);
    if (elapsedTime >= AmphenolPMSensor.getSampleWindowDt())
    {
        haveSample_ = true;
        overflowCnt_ = 0;
        AmphenolPMSensor.resetParticleAccum(LargeParticle);
        AmphenolPMSensor.resetParticleAccum(SmallParticle);
    }
    else
    {
        overflowCnt_++;
    }
}


AmphenolPMSensorDev AmphenolPMSensor(constants::DefaultAmphenolPMSensorParam);



