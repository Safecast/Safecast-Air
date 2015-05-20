#ifndef ALPHASENSE_GAS_SENSOR_H
#define ALPHASENSE_GAS_SENSOR_H
#include "gas_sensor_param.h"
#include "constants.h"
#include "fixed_vector.h"
#include "filter.h"

class GasSensorDev
{
    public:

        GasSensorDev();
        GasSensorDev(GasSensorParam param);

        GasSensorParam param() const;
        void setParam(GasSensorParam param);
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


class GasSensorDevVector : public FixedVector<GasSensorDev,constants::NumGasSensor>
{
    public:
        GasSensorDevVector(){};
        void initialize();
        void start();
        void stop();
        void sample();

    protected:
        void setupAnalogInput();
        static unsigned long getSampleDtUs();
        static void onTimerOverflow();
};


extern GasSensorDevVector GasSensors;

#endif
