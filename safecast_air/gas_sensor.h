#ifndef ALPHASENSE_GAS_SENSOR_H
#define ALPHASENSE_GAS_SENSOR_H
#include "gas_sensor_param.h"
#include "sensor_dev_vector.h"
#include "constants.h"
#include "fixed_vector.h"
#include "filter.h"
#include <Arduino.h>


class GasSensorDev
{
    public:

        GasSensorDev();
        GasSensorDev(GasSensorParam param);

        GasSensorParam param() const;
        void setParam(GasSensorParam param);
        String paramToString() const;

        void sample(unsigned long dt);

        float ppb() const;
        float ppbLowPass() const;

        float ppm() const;
        float ppmLowPass() const;

        int workingInt() const;
        int auxillaryInt() const;

        float working() const;
        float auxillary() const;
        
        float workingZeroed() const;
        float auxillaryZeroed() const;

        String gasName() const;
        GasType gasType() const;


        bool isActive() const;

        void initialize();

    protected:

        GasSensorParam param_;

        volatile int workingInt_;
        volatile int auxillaryInt_;

        volatile float working_;
        volatile float auxillary_;

        volatile float workingZeroed_;
        volatile float auxillaryZeroed_;

        volatile float ppb_;

        filter::LowPass ppbLowPassFilter_;

};



class GasSensorDevVector : public SensorDevVector<GasSensorDev,GasSensorParam,constants::NumGasSensor>
{
    public:
        GasSensorDevVector() {};
        GasSensorDevVector(const SamplingParam sampParam, const GasSensorParam devParam[]) 
            : SensorDevVector(sampParam, devParam) 
        { 
            timerCallback_ = GasSensorDevVector::onTimerOverflow;
        };

    protected:
        static void onTimerOverflow();

};


//class GasSensorDevVector : public FixedVector<GasSensorDev,constants::NumGasSensor>
//{
//    public:
//        static const uint8_t TimerPriority = 100;
//        GasSensorDevVector(){};
//        void initialize();
//        void start();
//        void stop();
//        void sample();
//
//    protected:
//        IntervalTimer timer_;
//        void setupAnalogInput();
//        static unsigned long getSampleDtUs();
//        static void onTimerOverflow();
//};


extern GasSensorDevVector GasSensors;


#endif
