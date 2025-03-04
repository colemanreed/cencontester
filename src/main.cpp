#include "./solenoids/solenoids.h"
#include "./motors/motorControl.h"
#include "./camera/camera.h"
#include "./controlpanel/controlpanel.h"

/*


Main control flow


*/


int completedCycles = 0;
int numCycles;

void setup() {
    // initialize all subsystems
    initLCD();
    initIO();
    initMotor();
    initSolenoids();
    initCamera();
    
    // wait to make sure initialization is okay

    // allow user to set cycles
    numCycles = getUserCycles();

    // wait for user 'start' input
    
    // set cycle counter to 0 on LCD

}

void loop() {
    while(completedCycles < numCycles) {
        // increment cycle counter by 1

        // begin spinning motor
        // wait for camera to detect 'dL' on keypad

        // spin motor counterclockwise
        // wait for camera to detect 'EC' on keypad

        // send signals to solenoids to enter code on keypad
        // wait for camera to detect 'opR' on keypad

        // spin motor clockwise
        // wait for break beam sensor to confirm lock bolt disengaged

        // if lock bolt disengaged
            // increment passed test counter by 1
            // spin motor counterclockwise (to engage lock)
        // else (lock bolt is still engaged)
            // increment failed test counter by 1
    }

    // testing is completed, display summary on LCD
    // wait for user to press start again
}