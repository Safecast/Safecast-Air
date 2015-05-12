#ifndef AMPHENOL_PM_SENSOR_H
#define AMPHENOL_PM_SENSOR_H

namespace amphenol
{

    struct PMSensorParam
    {
        int smallParticlePin;
        int largeParticlePin;
        float sensitivity;
        unsigned long sampleWindow;
    };

    class OccupancySensor 
    {
        // Notes  need to modifiy so that value will reset when not receiving regualar pulses.
        // This is basically a bug. Should separate the pin chage interrupt from the elapsed 
        // time check - perhaps run the elapsed time check on some sort of  timer interrupt.  

        public:

            OccupancySensor();
            void initialize(int pin, unsigned long sampleWindow, void (*pinChangeFunc)(void)); 
            void reset();
            void update();
            float value();
            float rate();
            unsigned long count();

        protected:

            int pin_;
            int pinState_;
            unsigned long sampleWindow_;

            unsigned long lastChangeTime_;
            unsigned long totalTimeAccum_;
            unsigned long highTimeAccum_;
            unsigned long lowTimeAccum_;
            unsigned long countAccum_;

            unsigned long totalTime_;
            unsigned long highTime_;
            unsigned long lowTime_;
            unsigned long count_;

            void resetAccum();
    };



    class PMSensorDev
    {
        // Amphenol Smart Dust Sensor SM-PWM-01A 

        public:

            PMSensorDev();
            PMSensorDev(PMSensorParam param);

            void setParam(PMSensorParam param);
            PMSensorParam param();

            void initialize();
            void update(bool small, bool large); 

            float smallParticleOccupancy();
            float largeParticleOccupancy();

            float smallParticleRate();
            float largeParticleRate();

            unsigned long smallParticleCount();
            unsigned long largeParticleCount();


        protected:

            PMSensorParam param_;

            OccupancySensor smallParticleSensor_;
            OccupancySensor largeParticleSensor_;

            static void onSmallParticlePinChange();
            static void onLargeParticlePinChange();


    };

} // namespace amphenol


extern amphenol::PMSensorDev PMSensor;

#endif


