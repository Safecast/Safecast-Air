#ifndef ALPHASENSE_GAS_SENSOR_H
#define ALPHASENSE_GAS_SENSOR_H
#include "gas_sensor_param.h"
#include "sensor_dev_vector.h"
#include "filter.h"
#include <Arduino.h>

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



template<unsigned int n>
class GasSensorDevVector: public SensorDevVector<GasSensorDev,GasSensorParam,n>
{
    public:
        GasSensorDevVector() 
        { };
        GasSensorDevVector(const SamplingParam sampParam, const GasSensorParam devParam[]) 
            : SensorDevVector<GasSensorDev,GasSensorParam,n>(sampParam, devParam) 
        { };

};

#endif
