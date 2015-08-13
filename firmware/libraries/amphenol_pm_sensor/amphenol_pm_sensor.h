#ifndef AMPHENOL_PM_SENSOR_H
#define AMPHENOL_PM_SENSOR_H
#include "amphenol_pm_sensor_param.h"
#include "occupancy_accum.h"
#include <Arduino.h>



//// Amphenol PM sensor parameters
//// --------------------------------------------------------------------------------------------
//const AmphenolPMSensorParam DefaultAmphenolPMSensorParam = 
//{  // smallParticlePin, largeParticlePin, sensitivity,  sampleWindowDt (ms),
//    32, 33, 1.5, 60000
//};


class AmphenolPMSensorDev
{
    // Amphenol Smart Dust Sensor SM-PWM-01A 

    public:

        static const unsigned long TimerPeriodUs = 100000; 
        static const uint8_t TimerPriority = 200;

        AmphenolPMSensorDev();
        AmphenolPMSensorDev(AmphenolPMSensorParam param);

        void setParam(AmphenolPMSensorParam param);
        AmphenolPMSensorParam param() const;

        void initialize();
        void start();
        void stop();
        void reset();

        void updateParticleAccum(ParticleType particleType);
        void resetParticleAccum(ParticleType particleType);

        float occupancy(ParticleType particleType) const;
        float pulseRate(ParticleType particleType) const;
        unsigned long pulseCount(ParticleType particleType) const;
        unsigned long getSampleWindowDt() const;
        float countPerCubicFt(ParticleType particleType) const;
        static bool haveSample();

    protected:

        AmphenolPMSensorParam param_;
        OccupancyAccum particleAccum_[NumParticleType];
        IntervalTimer timer_;

        static volatile bool haveSample_;
        static volatile unsigned long overflowCnt_;
        static void onTimerOverflow();
        template<ParticleType particleType>
        static void onPinChange();

};


extern AmphenolPMSensorDev AmphenolPMSensor;

template<ParticleType particleType>
void AmphenolPMSensorDev::onPinChange()
{
    AmphenolPMSensor.updateParticleAccum(particleType);
}



#endif


