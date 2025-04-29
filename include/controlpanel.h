#include <LiquidCrystal.h>
#include <Arduino.h>

void initControlPanel();
int getUserCycles();
void updateCycles();
void updateLCD(const char textLines[]);
int getStartState();
void updateLED(int status);
int getDownState();
int getUpState();