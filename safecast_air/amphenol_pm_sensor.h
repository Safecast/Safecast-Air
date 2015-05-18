#ifndef AMPHENOL_PM_SENSOR_H
#define AMPHENOL_PM_SENSOR_H

namespace amphenol
{

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


    struct PMSensorParam
    {
        int smallParticlePin;
        int largeParticlePin;
        float sensitivity;
        unsigned int sampleWindowDt; // ms
    };

    class PMSensorDev
    {
        // Amphenol Smart Dust Sensor SM-PWM-01A 

        public:

            static unsigned long TimerPeriodUs;

            PMSensorDev();
            PMSensorDev(PMSensorParam param);

            void setParam(PMSensorParam param);
            PMSensorParam param();

            void initialize();
            void updateSmallParticleSensor();
            void updateLargeParticleSensor();

            void resetSmallParticleAccum();
            void resetLargeParticleAccum();

            float smallParticleOccupancy();
            float largeParticleOccupancy();

            float smallParticleRate();
            float largeParticleRate();

            unsigned long smallParticleCount();
            unsigned long largeParticleCount();

            unsigned int getSampleWindowDt();

        protected:

            PMSensorParam param_;

            OccupancyAccum smallParticleAccum_;
            OccupancyAccum largeParticleAccum_;

            static void onSmallParticlePinChange();
            static void onLargeParticlePinChange();

            static void onTimerOverFlow();


    };

} // namespace amphenol


extern amphenol::PMSensorDev PMSensor;

#endif


