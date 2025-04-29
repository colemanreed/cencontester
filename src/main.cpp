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

    // all LEDs on during setup
    updateLED(4);
    
    Serial.println("Set cycle loop begin");
    // allow user to set cycles
    while(!getStartState()) {
        updateCycles();
    }
    Serial.println("Set cycle loop end");

    numCycles = getUserCycles();
    Serial.println(numCycles);
    
    // TODO set cycle counter to 0 on LCD

    completedCycles = passedCycles = failedCycles = 0;
}

void loop() {
    Serial.println("Main program loop begin");

    // initial charging to be ran once
    Serial.println("Initial charging");
    motorClockwise(5000);
    delay(500);
    motorCounterClockwise(5000);
    delay(500);

    while(completedCycles < numCycles) {
        // in progress LED on
        updateLED(3);
        String response;
        Serial.println("Starting cycle " + (String)(completedCycles + 1));
        updateLCD("Cycle: " + (completedCycles+1));

        // begin spinning motor
        motorClockwise(5000);
        delay(500);
        motorCounterClockwise(5000);
        delay(500);
        motorClockwise(5000);

        Serial.println("Motor done spinning");

        // wait for camera to detect 'EC' on keypad
        Serial.println("TAKE_PIC");
        delay(2000);
        Serial.println("PROCESS_PIC");
        while(Serial.available()) {
            response = Serial.readStringUntil('\n');
            response.trim();
        }
        // response = "['E']"; // DEBUG
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
        // response = "['O']"; // DEBUG
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

        // spin motor counterclockwise
        // wait for break beam sensor to confirm lock bolt disengaged
        motorCounterClockwise(2000);

        delay(2000);
        if(readBoltDetector()) {
            passedCycles++;
            updateLED(1);
            delay(2000);
        } else {
            Serial.println("Failed on bolt detector");
            failedCycles++;
            updateLED(2);
            delay(2000);
        }

        // extend bolt
        motorClockwise(2000);

        // increment cycle counter by 1
        completedCycles++;

        // while(!getStartState()); //DEBUG

        Serial.println("Completed Cycles: " + (String)completedCycles);
        Serial.println("Passed Cycles: " + (String)passedCycles);
        Serial.println("Failed Cycles: " + (String)failedCycles);
    }

    // testing is completed
    // TODO display summary on LCD

    Serial.println("Completed Cycles: " + (String)completedCycles);
    Serial.println("Passed Cycles: " + (String)passedCycles);
    Serial.println("Failed Cycles: " + (String)failedCycles);
    Serial.println("Main program loop end");
    while(1);
}
