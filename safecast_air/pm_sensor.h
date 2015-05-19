#ifndef AMPHENOL_PM_SENSOR_H
#define AMPHENOL_PM_SENSOR_H
#include "pm_sensor_param.h"




class OccupancyAccum 
{
    public:

        OccupancyAccum();
        void initialize(int pin); 
        void resetValues();
        float value();
        unsigned long count();
        
        // Interrupt functions
        void onPinChange();
        void onResetTimer();

    protected:

        int pin_;
        volatile int pinState_;

        volatile unsigned long lastChangeTime_;
        volatile unsigned long highTimeAccum_;
        volatile unsigned long lowTimeAccum_;
        volatile unsigned long countAccum_;

        volatile unsigned long count_;
        volatile unsigned long highTime_;
        volatile unsigned long lowTime_;
        volatile unsigned long totalTime_;

};




class PMSensorDev
{
    // Amphenol Smart Dust Sensor SM-PWM-01A 

    public:

        static const unsigned long TimerPeriodUs = 100000; 

        PMSensorDev();
        PMSensorDev(PMSensorParam param);

        void setParam(PMSensorParam param);
        PMSensorParam param();

        void initialize();

        void updateParticleAccum(ParticleType particleType);
        void resetParticleAccum(ParticleType particleType);

        float occupancy(ParticleType particleType);
        float rate(ParticleType particleType);

        unsigned long count(ParticleType particleType);

        unsigned int getSampleWindowDt();

    protected:

        PMSensorParam param_;
        OccupancyAccum particleAccum_[NumParticleType];

        static void onSmallParticlePinChange();
        static void onLargeParticlePinChange();
        //static void onPinChange(ParticleType particleType);

        static void test();
        static void onTimerOverFlow();

};


extern PMSensorDev PMSensor;

#endif


