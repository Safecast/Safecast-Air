#ifndef TMP_SENSOR_H
#define TMP_SENSOR_H
#include "tmp_sensor_param.h"
#include "sensor_dev_vector.h"
#include "filter.h"
#include <Arduino.h>


class TmpSensorDev
{
    public:

        TmpSensorDev();
        TmpSensorDev(TmpSensorParam param);

        TmpSensorParam param() const;
        void setParam(TmpSensorParam param);

        void sample(unsigned long dt);

        float value() const;
        float valueLowPass() const;

        int rawInt() const;
        float rawVolt() const;

        bool isActive() const;

        void initialize();

    private:

        TmpSensorParam param_;
        filter::LowPass lowPassFilter_;
        volatile int rawInt_;
        volatile float rawVolt_;
        volatile float value_;

};


template<unsigned int n>
class TmpSensorDevVector: public SensorDevVector<TmpSensorDev,TmpSensorParam,n>
{
    public:
        TmpSensorDevVector() 
        { };
        TmpSensorDevVector(const SamplingParam sampParam, const TmpSensorParam devParam[]) 
            : SensorDevVector<TmpSensorDev,TmpSensorParam,n>(sampParam, devParam) 
        { };

};



#endif
