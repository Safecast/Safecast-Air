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

        GasSensorParam param();
        void setParam(GasSensorParam param);
        void sample(float dt);

        float ppb();
        float ppbLowPass();

        float ppm();
        float ppmLowPass();

        int workingInt();
        int auxillaryInt();

        float working();
        float auxillary();
        
        float workingZeroed();
        float auxillaryZeroed();


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

        void initializeValues();
};


// May create gas sensor array class .. to handle array of gas sensors.

class GasSensorDevVector : public FixedVector<GasSensorDev,constants::NumGasSensor>
{
    public:
        GasSensorDevVector(){};
        void initialize();
    protected:
        void setupAnalogInput();

};

extern GasSensorDevVector GasSensors;

#endif
