#ifndef FILTER_H
#define FILTER_H

namespace filter
{

    struct LowPassParam
    {
        float cutoffFreq;
        float initialValue;
        unsigned int order;
    };

    // First order low-pass filter
    class LowPass
    {
        public:

            static const unsigned int MaxOrder_ = 5; 

            LowPass(float cutoff_freq=1.0, unsigned int order=1, float value=0.0);
            LowPass(LowPassParam param);

            void setParam(float cutoff_freq, unsigned int order, float value);
            void setParam(LowPassParam param);
            LowPassParam param();

            float cutoffFreq();
            void setCutoffFreq(float cutoffFreq);

            float initialValue();
            void setInitialValue(float initialValue);

            float order();
            float setOrder(unsigned int order);

            void update(float value, float dt);
            void reset();
            float value();

        protected:

            LowPassParam param_;
            volatile float state_[MaxOrder_+1]; 
            void initializeState();
    };

} // namespace filter

#endif


