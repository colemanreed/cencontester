#include <Arduino.h>
#include "solenoids.h"
#include "motorControl.h"
#include "controlpanel.h"
#include <string>

/*

Main control flow

*/

int completedCycles;
int passedCycles;
int numCycles;
int failedCycles;

// for updateLED:
// 0 - none
// 1 - success
// 2 - fail
// 3 - progress
// else - all on

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
    initBoltDetector();
    Serial.println("Bolt detector initialized");
    
    // wait to make sure initialization is okay
    delay(50);

    updateLED(4);
    
    Serial.println("Set cycle loop begin");
    // allow user to set cycles
    while(!getStartState()) {
        updateCycles();
    }
    Serial.println("Set cycle loop end");
    numCycles = getUserCycles();
    Serial.println(numCycles);
    // set cycle counter to 0 on LCD
    // currently only displays num of total cycles

    completedCycles = passedCycles = failedCycles = 0;

}

void loop() {
    Serial.println("Main program loop begin");
    Serial.println("Initial charging");
    motorClockwise(5000);
    delay(500);
    motorCounterClockwise(5000);
    delay(500);
    // motorClockwise(5000);
    // delay(500);
    // motorCounterClockwise(5000);
    // delay(500);
    // motorClockwise(5000);
    while(completedCycles < numCycles) {
        updateLED(3);
        String response;
        Serial.println("Startin cycle " + (String)(completedCycles + 1));
        updateLCD("Cycle: " + (completedCycles+1));
        // begin spinning motor
        motorClockwise(5000);
        delay(500);
        motorCounterClockwise(5000);
        delay(500);
        motorClockwise(5000);

        Serial.println("Ready to go");

        // wait for camera to detect 'EC' on keypad
        Serial.println("TAKE_PIC");
        delay(2000);
        Serial.println("PROCESS_PIC");
        while(Serial.available()) {
            response = Serial.readStringUntil('\n');
            response.trim();
        }
        response = "['E']"; // DEBUG
        if(response.indexOf('E') == -1) {
            Serial.println("Trying EC read again");
            Serial.println("PROCESS_PIC");
            while(Serial.available()) {
                response = Serial.readStringUntil('\n');
                response.trim();
            }
            if(response.indexOf('E') == -1) {
                // Failure
                failedCycles++;
                completedCycles++;
                updateLED(2);
                Serial.println("Failed on EC");
                Serial.println("Completed Cycles: " + (String)completedCycles);
                Serial.println("Passed Cycles: " + (String)passedCycles);
                Serial.println("Failed Cycles: " + (String)failedCycles);
                delay(5000);
                continue;
            }
        }

        delay(5000);

        // send signals to solenoids to enter code on keypad
        // wait for camera to detect 'opR' on keypad
        Serial.println("Entering code");
        solenoidsEnterCode();

        Serial.println("TAKE_PIC");
        delay(2000);
        Serial.println("PROCESS_PIC");
        while(Serial.available()) {
            response = Serial.readStringUntil('\n');
            response.trim();
        }
        response = "['O']"; // DEBUG
        if(response.indexOf('O') == -1) {
            Serial.println("Trying Opr read again");
            Serial.println("PROCESS_PIC");
            while(Serial.available()) {
                response = Serial.readStringUntil('\n');
                response.trim();
            }
            if(response.indexOf('O') == -1) {
                // Failure
                failedCycles++;
                completedCycles++;
                updateLED(2);
                Serial.println("Failed on Opr");
                Serial.println("Completed Cycles: " + (String)completedCycles);
                Serial.println("Passed Cycles: " + (String)passedCycles);
                Serial.println("Failed Cycles: " + (String)failedCycles);
                delay(5000);
                continue;
            }
        }

        delay(5000);

        // while(!getStartState()) {
        //     while(getUpState()) {
        //         infiniteClockwise(getUpState);
        //     } while(getDownState()) {
        //         infiniteCounterClockwise(getDownState);
        //     }
        // }
        // spin motor clockwise
        // wait for break beam sensor to confirm lock bolt disengaged
        motorCounterClockwise(2000);
        // if lock bolt disengaged
            // increment passed test counter by 1
            // spin motor counterclockwise (to engage lock)
        // else (lock bolt is still engaged)
            // increment failed test counter by 1

            // while(!getStartState()) {
            //     while(getUpState()) {
            //         infiniteClockwise(getUpState);
            //     } while(getDownState()) {
            //         infiniteCounterClockwise(getDownState);
            //     }
            // }
        delay(2000);
        motorClockwise(2000);
        if(!getUpState()) {
            passedCycles++;
            updateLED(1);
            delay(5000);
        } else {
            Serial.println("Failed on bolt detector");
            failedCycles++;
            updateLED(2);
            delay(5000);
        }



        // increment cycle counter by 1
        completedCycles++;

        while(!getStartState());
        Serial.println("Completed Cycles: " + (String)completedCycles);
        Serial.println("Passed Cycles: " + (String)passedCycles);
        Serial.println("Failed Cycles: " + (String)failedCycles);
    }

    // testing is completed, display summary on LCD
    // wait for user to press start again

    Serial.println("Completed Cycles: " + (String)completedCycles);
    Serial.println("Passed Cycles: " + (String)passedCycles);
    Serial.println("Failed Cycles: " + (String)failedCycles);
    Serial.println("Main program loop end");
    while(1);
}
