#ifndef SENSOR_DEV_VECTOR_H
#define SENSOR_DEV_VECTOR_H
#include "sampling_param.h"
#include "fixed_vector.h"


template<typename Dev, typename DevParam, unsigned int n>
class SensorDevVector  : public FixedVector<Dev,n>
{
    public:

        SensorDevVector(){};

        SensorDevVector(const SamplingParam sampParam, const DevParam devParam[n])
        {
            setParam(sampParam,devParam);
        };

        void initialize() 
        {
            setupAnalogInput();
            timer_.priority(sampParam_.timerPriority);
        };

        void start()
        {
            for (auto &sensor: *this)
            {
                sensor.initialize();
            }
            timer_.begin(timerCallback_,  getSampleDtUs());
        };

        void stop()
        {
            timer_.end();
        };

        void sample()
        {
            for (auto &sensor : *this)
            {
                sensor.sample(sampParam_.sampleDt);
            }
        }

        void setParam(const SamplingParam sampParam, const DevParam devParam[n])
        {
            setSamplingParam(sampParam);
            setDevParam(devParam);
        }

        void setDevParam(const DevParam devParam[n])
        {
            for (int i=0; i<n; i++)
            {
                FixedVector<Dev,n>::set(i,Dev(devParam[i]));
            }
        }

        void setSamplingParam(const SamplingParam sampParam)
        {
            sampParam_ = sampParam;
        }

        void setTimerCallback(void (*timerCallback)())
        {
            timerCallback_ = timerCallback;
        }

    protected:

        SamplingParam sampParam_;
        IntervalTimer timer_;

        static void dummyTimerCallback() { }; 

        void (*timerCallback_)() = dummyTimerCallback; 

        void setupAnalogInput()
        {
            analogReadRes(sampParam_.ainResolution);
            analogReadAveraging(sampParam_.ainAveraging);
            analogReference(INTERNAL);
        };

        unsigned long getSampleDtUs()
        {
            return 1000ul*sampParam_.sampleDt;
        };

};
#endif
