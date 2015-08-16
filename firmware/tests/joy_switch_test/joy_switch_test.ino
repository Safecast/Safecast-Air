#include <Streaming.h>
#include <util/atomic.h>

const int joyAinPin = A15;
const int joyIntPin = 25;

volatile bool newJoyEvent = false;
volatile int joyEventCnt = 0;
volatile int joyEventVal = 0;


void joyIntCallback();

void setup()
{
    Serial.begin(9600);
    analogReference(DEFAULT);
    analogReadRes(12);
    analogReadAveraging(10);
    pinMode(joyIntPin,INPUT);
    attachInterrupt(joyIntPin,joyIntCallback,RISING);
    interrupts();
}

void loop()
{
    bool newJoyEventCopy = false;
    int joyEventCntCopy = 0;
    int joyEventValCopy = 0;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        newJoyEventCopy = newJoyEvent;
        joyEventCntCopy = joyEventCnt;
        joyEventValCopy = joyEventVal;
        newJoyEvent = false;
    }

    if (newJoyEventCopy)
    {
        Serial << "joyEvent cnt: " << joyEventCnt << ", val: " << joyEventVal << endl;
    }

    delay(100);
}

void joyIntCallback()
{
        newJoyEvent = true;
        joyEventCnt++;
        joyEventVal = analogRead(joyAinPin);

}
