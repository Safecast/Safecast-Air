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

        int workingInt_;
        int auxillaryInt_;

        float working_;
        float auxillary_;

        float workingZeroed_;
        float auxillaryZeroed_;

        float ppb_;
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
