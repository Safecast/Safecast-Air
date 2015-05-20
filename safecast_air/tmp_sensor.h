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


// Perhaps can combine common stuff with GasSensorDevVector ... maybe make a SensorDevVector base class

class TmpSensorDevVector : public FixedVector<TmpSensorDev,constants::NumTmpSensor>
{
    public:
        static const uint8_t TimerPriority = 150;
        TmpSensorDevVector(){};
        void initialize();
        void start();
        void stop();
        void sample();

    protected:
        IntervalTimer timer_;
        void setupAnalogInput();
        static unsigned long getSampleDtUs();
        static void onTimerOverflow();
};


extern TmpSensorDevVector TmpSensors;

#endif
