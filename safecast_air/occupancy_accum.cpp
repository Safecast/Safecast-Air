#include "occupancy_accum.h"
#include <limits.h>
#include "Arduino.h"
#include <util/atomic.h>

inline unsigned long getDt(unsigned long t0, unsigned long t1)
{
    unsigned long dt = 0;
    if (t1 < t0)
    {
        dt = t1 + (ULONG_MAX - t0);
    }
    else
    {
        dt = t1 - t0;
    }
    return dt;
}


OccupancyAccum::OccupancyAccum() { }


void OccupancyAccum::initialize(int pin)
{
    pin_ = pin;
    pinMode(pin_, INPUT);
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


float  OccupancyAccum::value()
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


unsigned long OccupancyAccum::count()
{
    unsigned long countCpy = 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        countCpy = count_;
    }
    return count_;
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
