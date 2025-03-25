#include "./solenoids/solenoids.h"
#include "./motors/motorControl.h"
#include "./camera/camera.h"
#include "./controlpanel/controlpanel.h"

/*


Main control flow


*/


int completedCycles;
int passedCycles;
int numCycles;
int failedCycles;

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("In setup");
    // initialize all subsystems
    initControlPanel();
    Serial.println("Control panel initialized");
    initMotor();
    Serial.println("Motor initialized");
    initSolenoids();
    Serial.println("Solenoids initialized");
    //initCamera();
    initBoltDetector();
    Serial.println("Bolt detector initialized");
    
    // wait to make sure initialization is okay
    delay(50);
    
    // wait for user 'start' input

    Serial.println("Set cycle loop begin");
    // allow user to set cycles
    while(!getStartState()) {
        updateCycles();
    }
    Serial.println("Set cycle loop end");
    numCycles = getUserCycles();

    // set cycle counter to 0 on LCD
    // currently only displays num of total cycles

    completedCycles = passedCycles = failedCycles = 0;
}

void loop() {
    Serial.println("Main program loop begin");
    while(completedCycles < numCycles) {

        updateLCD("Cycle: " + (completedCycles+1));
        // begin spinning motor
        motorClockwise(50000);
        // wait for camera to detect 'dL' on keypad

        // spin motor counterclockwise
        // wait for camera to detect 'EC' on keypad
        motorCounterClockwise(5000);
        // send signals to solenoids to enter code on keypad
        // wait for camera to detect 'opR' on keypad
        solenoidsEnterCode();
        // spin motor clockwise
        // wait for break beam sensor to confirm lock bolt disengaged
        motorClockwise(5000);
        // if lock bolt disengaged
            // increment passed test counter by 1
            // spin motor counterclockwise (to engage lock)
        // else (lock bolt is still engaged)
            // increment failed test counter by 1

        if(readBoltDetector()) {
            passedCycles++;
            motorCounterClockwise(5000);
        } else {
            failedCycles++;
        }

        // increment cycle counter by 1
        completedCycles++;
    }

    // testing is completed, display summary on LCD
    // wait for user to press start again


    Serial.println("Main program loop end");
    while(1);
}