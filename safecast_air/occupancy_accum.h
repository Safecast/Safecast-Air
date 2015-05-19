#ifndef OCCUPANCY_ACCUM_H
#define OCCUPANCY_ACCUM_H

class OccupancyAccum 
{
    public:

        OccupancyAccum();
        void initialize(int pin); 
        void resetValues();
        float value();
        unsigned long count();
        
        // Interrupt functions
        void onPinChange();
        void onResetTimer();

    protected:

        int pin_;
        volatile int pinState_;

        volatile unsigned long lastChangeTime_;
        volatile unsigned long highTimeAccum_;
        volatile unsigned long lowTimeAccum_;
        volatile unsigned long countAccum_;

        volatile unsigned long count_;
        volatile unsigned long highTime_;
        volatile unsigned long lowTime_;
        volatile unsigned long totalTime_;

};

#endif
