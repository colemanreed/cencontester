#include <Arduino.h>
void initMotor();
void initBoltDetector();
int readBoltDetector();
void motorClockwise(int onTime);
void motorCounterClockwise(int onTime);
void infiniteClockwise(bool val);
void infiniteCounterClockwise(bool val);