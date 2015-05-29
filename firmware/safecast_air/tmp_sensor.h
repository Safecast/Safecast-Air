#ifndef TMP_SENSOR_H
#define TMP_SENSOR_H
#include "tmp_sensor_param.h"
#include "constants.h"
#include "fixed_vector.h"
#include "filter.h"
#include <Arduino.h>


class TmpSensorDev
{
    public:

        TmpSensorDev();
        TmpSensorDev(TmpSensorParam param);

        TmpSensorParam param() const;
        void TmpParam(GasSensorParam param);
        String paramToString() const;

        void sample(unsigned long dt);

        float value() const;
        float valueLowPass() const;

        float offset() const;
        float offsetTmp() const;

        bool isActive() const;

        void initialize();

    private:

        TmpSensorParam param_;
        filter::LowPass ppbLowPassFilter_;

};


class TmpSensorDevVector : public SensorDevVector<TmpSensorDev,TmpSensorParam,constants::NumTmpSensor>
{
    public:
        TmpSensorDevVector() {};
        TmpSensorDevVector(const SamplingParam sampParam, const TmpSensorParam devParam[]) 
            : SensorDevVector(sampParam, devParam) 
        { 
            setTimerCallback(TmpSensorDevVector::onTimerOverflow);
        };

    protected:
        static void onTimerOverflow();

};


extern TmpSensorDevVector TmpSensors;

#endif
