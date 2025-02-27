#include "./solenoids/solenoids.h"
#include "./motors/motorControl.h"
#include "./camera/camera.h"

/*


Main control flow


*/

//control panel
static const int LCD_RST_PIN = 7;
static const int LCD_ENA_PIN = 8;
static const int LCD_D4_PIN = 9;
static const int LCD_D5_PIN = 10;
static const int LCD_D6_PIN = 11;
static const int LCD_D7_PIN = 12;

static const int CYCLE_UP_PIN = 13;
static const int CYCLE_DOWN_PIN = 14;

static const int LED_RED_PIN = 15;
static const int LED_YELLOW_PIN = 16;
static const int LED_GREEN_PIN = 17;

void initLCD();
void initIO(); // for buttons and LEDs

int completedCycles = 0;
int numCycles;

void setup() {
    // initialize all subsystems
        // wait to make sure initialization is okay

    // allow user to set cycles
    // numCycles = getUserCycles();

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