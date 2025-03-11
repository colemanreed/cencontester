/*


Code for motor controls


*/

#include "motorControl.h"

//motor controller
static const int MTR_STEP_PIN = 13;
static const int MTR_DIR_PIN = 14;
static const int MTR_ENA_PIN = 15;
static const int BRK_BEAM_PIN = 38;

unsigned long startTime;
unsigned long stopTime;

void initMotor() {
    pinMode(MTR_STEP_PIN, OUTPUT);
    pinMode(MTR_DIR_PIN, OUTPUT);
    pinMode(MTR_ENA_PIN, OUTPUT);

    digitalWrite(MTR_ENA_PIN, LOW);

    // high is counterclockwise, low is clockwise
    digitalWrite(MTR_DIR_PIN, HIGH);

}

void initBoltDetector() {
    pinMode(BRK_BEAM_PIN, INPUT);
}

int readBoltDetector() {
    return digitalRead(BRK_BEAM_PIN);
}

// onTime in ms
void motorClockwise(int onTime) {
    digitalWrite(MTR_DIR_PIN, LOW);

    startTime = millis();
    stopTime = startTime;

    while((stopTime - startTime) <= onTime) {
        digitalWrite(MTR_STEP_PIN, HIGH);
        delay(1);
        digitalWrite(MTR_STEP_PIN, LOW);
        delay(1);

        stopTime = millis();
    }
}

void motorCounterClockwise(int onTime) {
    digitalWrite(MTR_DIR_PIN, HIGH);

    startTime = millis();
    stopTime = startTime;

    while((stopTime - startTime) <= onTime) {
        digitalWrite(MTR_STEP_PIN, HIGH);
        delay(1);
        digitalWrite(MTR_STEP_PIN, LOW);
        delay(1);
        
        stopTime = millis();
    }
}