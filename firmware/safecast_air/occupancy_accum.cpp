#include "occupancy_accum.h"
#include "utility.h"
#include <Arduino.h>
#include <util/atomic.h>



OccupancyAccum::OccupancyAccum() { }


void OccupancyAccum::initialize(int pin)
{
    pin_ = pin;
    pinMode(pin_, INPUT_PULLUP);
    resetValues();
}


void OccupancyAccum::resetValues()
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        totalTime_ = 0;
        highTime_ = 0;
        lowTime_ = 0;
        count_ = 0;
        highTimeAccum_ = 0;
        lowTimeAccum_ = 0;
        countAccum_ = 0;
        pinState_ = digitalRead(pin_);
        lastChangeTime_ = micros();
    }
}


float  OccupancyAccum::value() const
{
    float value = 0.0;
    unsigned long lowTimeCpy = 0;
    unsigned long totalTimeCpy = 0;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        lowTimeCpy = lowTime_;
        totalTimeCpy = totalTime_;
    }
    if (totalTimeCpy != 0)
    {
        value = float(lowTimeCpy)/float(totalTimeCpy);
    }
    return value;
}


unsigned long OccupancyAccum::count() const
{
    unsigned long countCpy = 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        countCpy = count_;
    }
    return countCpy;
}


void OccupancyAccum::onResetTimer()
{
    // -------------------------------
    // Called in timer interrupt only
    // -------------------------------
    unsigned long currentTime = micros();
    unsigned long dt = getDt(lastChangeTime_, currentTime);

    if (pinState_ == HIGH)
    {
        highTimeAccum_ += dt;
    }
    else
    {
        lowTimeAccum_ += dt;
    }

    lastChangeTime_ = currentTime;
    pinState_ = digitalRead(pin_);

    count_  = countAccum_;
    highTime_ = highTimeAccum_;
    lowTime_ = lowTimeAccum_;
    totalTime_ = highTimeAccum_ + lowTimeAccum_;

    highTimeAccum_ = 0;
    lowTimeAccum_ = 0;
    countAccum_ = 0;
}

void OccupancyAccum::onPinChange()
{
    // ------------------------------------
    // Called in pinChange interrupt only
    // ------------------------------------

    int newPinState = digitalRead(pin_);
    unsigned long currentTime = micros();
    unsigned long dt = getDt(lastChangeTime_, currentTime);

    if (pinState_ != newPinState)
    {

        // Update  time and count accumulators
        if (newPinState == HIGH)
        {
            lowTimeAccum_  += dt;
            countAccum_ += 1;
        }
        else
        {
            highTimeAccum_ += dt;
        }
        pinState_ = newPinState;
        lastChangeTime_ = currentTime;
    }
}
