#include "filter.h"
#include <Arduino.h>

namespace filter
{
    // Public methods
    // ------------------------------------------------------------------------
    LowPass::LowPass(float cutoffFreq,  unsigned int order, float value)
    {
        setParam(cutoffFreq, order, value);
        initializeState();
    }

    LowPass::LowPass(LowPassParam param)
    {
        setParam(param);
        initializeState();
    }

    void LowPass::setParam(float cutoffFreq, unsigned int order, float value)
    {
        param_.initialValue = value;
        param_.cutoffFreq = cutoffFreq;
        param_.order = min(order,MaxOrder_);
    }

    void LowPass::setParam(LowPassParam param)
    {
        param_ = param;
    }

    LowPassParam LowPass::param()
    {
        return param_;
    }

    float LowPass::cutoffFreq()
    {
        return param_.cutoffFreq;
    }

    void LowPass::setCutoffFreq(float cutoffFreq)
    {
        param_.cutoffFreq = cutoffFreq;
    }

    float LowPass::initialValue()
    {
        return param_.initialValue;
    }

    void LowPass::setInitialValue(float initialValue)
    {
        param_.initialValue = initialValue;
    }

    float LowPass::order()
    {
        return param_.order;
    }

    float LowPass::setOrder(unsigned int order)
    {
        param_.order = order;
    }

    void LowPass::update(float value, float dt)
    {
        float rc = 1.0/(2.0*M_PI*param_.cutoffFreq);
        float alpha = dt/(rc + dt);
        state_[0] = value;
        for (int i=1; i<=param_.order; i++)
        {
            state_[i] = (1.0 - alpha)*state_[i] + alpha*state_[i-1]; 
        }
    }

    void LowPass::reset()
    {
        initializeState();
    }

    float LowPass::value() const
    {
        return state_[param_.order];
    }

    // Protected methods
    // ------------------------------------------------------------------------

    void LowPass::initializeState()
    {
        for (int i=0; i<MaxOrder_+1; i++)
        {
            state_[i] = param_.initialValue;
        }
    }
}
