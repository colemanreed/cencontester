#include <Arduino.h>
void initMotor();
void initBoltDetector();
int readBoltDetector();
void motorClockwise(int onTime, int dutyCycle);
void motorCounterClockwise(int onTime, int dutyCycle);