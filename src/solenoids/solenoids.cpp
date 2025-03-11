/*


Code for solenoids (button pusher)


*/

#include <Arduino.h>

//button pusher
static const int SOLENOID0_PIN = 10; 
static const int SOLENOID2_PIN = 11; 
static const int SOLENOID5_PIN = 12; 

void initSolenoids() {
    pinMode(SOLENOID0_PIN, OUTPUT);
    pinMode(SOLENOID2_PIN, OUTPUT);
    pinMode(SOLENOID5_PIN, OUTPUT);

}
// solenoid for keypad button '0'
void extendSolenoid0() {
    digitalWrite(SOLENOID0_PIN, HIGH);
    delay(500);
    digitalWrite(SOLENOID0_PIN, LOW);
    delay(500);
} 

// solenoid for keypad button '2'
void extendSolenoid2() {
    digitalWrite(SOLENOID2_PIN, HIGH);
    delay(500);
    digitalWrite(SOLENOID2_PIN, LOW);
    delay(500);
}

// solenoid for keypad button '5'
void extendSolenoid5() {
    digitalWrite(SOLENOID5_PIN, HIGH);
    delay(500);
    digitalWrite(SOLENOID5_PIN, LOW);
    delay(500);
} 

// code is 50-25-50
void solenoidsEnterCode(){
    extendSolenoid5();
    extendSolenoid0();
    extendSolenoid2();
    extendSolenoid5();
    extendSolenoid5();
    extendSolenoid0();

}