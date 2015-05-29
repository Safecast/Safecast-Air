#ifndef AMPHENOL_PM_SENSOR_H
#define AMPHENOL_PM_SENSOR_H
#include "pm_sensor_param.h"
#include "occupancy_accum.h"
#include <Arduino.h>


class PMSensorDev
{
    // Amphenol Smart Dust Sensor SM-PWM-01A 

    public:

        static const unsigned long TimerPeriodUs = 100000; 
        static const uint8_t TimerPriority = 200;

        PMSensorDev();
        PMSensorDev(PMSensorParam param);

        void setParam(PMSensorParam param);
        PMSensorParam param() const;

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

        PMSensorParam param_;
        OccupancyAccum particleAccum_[NumParticleType];
        IntervalTimer timer_;

        static volatile bool haveSample_;
        static volatile unsigned long overflowCnt_;
        static void onTimerOverflow();
        template<ParticleType particleType>
        static void onPinChange();

};


extern PMSensorDev PMSensor;

template<ParticleType particleType>
void PMSensorDev::onPinChange()
{
    PMSensor.updateParticleAccum(particleType);
}



#endif


