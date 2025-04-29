/*


Code for control panel


*/

#include "controlpanel.h"

//button pusher


static const int CYCLE_UP_PIN = 20;
static const int CYCLE_DOWN_PIN = 19;

static const int LED_RED_PIN = 1;
static const int LED_YELLOW_PIN = 2;
static const int LED_GREEN_PIN = 39;

static const int START_PIN = 17;
static const int STOP_PIN = 21;


static const int LCD_RST_PIN = 38;
static const int LCD_ENA_PIN = 42;
static const int LCD_D4_PIN = 35;
static const int LCD_D5_PIN = 37;
static const int LCD_D6_PIN = 40;
static const int LCD_D7_PIN = 41;

// Variables for button states
bool upbuttonState = false;         // Current state of the "increase" button
bool lastUpbuttonState = false;     // Previous state of the "increase" button
bool downbuttonState = false;       // Current state of the "decrease" button
bool lastDownbuttonState = false;   // Previous state of the "decrease" button
int cycleSet = 50; // Initial value

// Debouncing variables
unsigned long lastDebounceTimeUp = 0;       // Last debounce time for "increase"
unsigned long lastDebounceTimeDown = 0;     // Last debounce time for "decrease"
const unsigned long debounceDelay = 50;     // Debounce delay (50ms)



void initControlPanel() {

    //LiquidCrystal lcd(LCD_RST_PIN, LCD_ENA_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);


    //lcd.begin(16, 2);
    // lcd.clear();
    // lcd.setCursor(0, 0);

    // lcd.print("Number of cycles "); // Print a message to the LCD

    //Initialize the pushbutton pins as inputs
    pinMode(CYCLE_UP_PIN, INPUT);
    pinMode(CYCLE_DOWN_PIN, INPUT);
    pinMode(START_PIN, INPUT);
    pinMode(STOP_PIN, INPUT);

    Serial.println("Im in control panel setup");

    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_YELLOW_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);

    Serial.println("Leaving control panel setup");

    //Display the initial cycle value
    // lcd.setCursor(0, 1);
    // lcd.print(cycleSet);
}

int getUserCycles() {
    return cycleSet;
}

int getStartState() {
    return digitalRead(START_PIN);
}

int getUpState() {
    return digitalRead(CYCLE_UP_PIN);
}

int getDownState() {
    return digitalRead(CYCLE_DOWN_PIN);
}

// Function to update the LCD
void updateLCD(const char textLines[]) {
    // lcd.setCursor(0, 1);  // Position to display the count
    // lcd.print("                "); // Clear the line
    // lcd.setCursor(0, 1);
    // lcd.print(textLines);   // Display the string
}

void updateCycles() {
    // Read the current button states
    bool currentUpButtonState = digitalRead(CYCLE_UP_PIN);     // Active HIGH
    bool currentDownButtonState = digitalRead(CYCLE_DOWN_PIN); // Active HIGH

    // Debounce logic for "increase" button
    if (currentUpButtonState != lastUpbuttonState) {
        lastDebounceTimeUp = millis(); // Update debounce timer
    }

    if ((millis() - lastDebounceTimeUp) > debounceDelay) {
        if (currentUpButtonState && !upbuttonState) { // Rising edge detection
        cycleSet++;
        updateLCD("Cycles to test: " + cycleSet);
        }
        upbuttonState = currentUpButtonState;
    }

    // Debounce logic for "decrease" button
    if (currentDownButtonState != lastDownbuttonState) {
        lastDebounceTimeDown = millis(); // Update debounce timer
    }

    if ((millis() - lastDebounceTimeDown) > debounceDelay) {
        if (currentDownButtonState && !downbuttonState) { // Rising edge detection
        cycleSet--;
        if (cycleSet < 0) cycleSet = 0; // Prevent negative count
        updateLCD("Cycles to test: " + cycleSet);
        }
        downbuttonState = currentDownButtonState;
    }

    // Save the last button states for the next loop
    lastUpbuttonState = currentUpButtonState;
    lastDownbuttonState = currentDownButtonState;

}

// 0 - none
// 1 - success
// 2 - in progress
// 3 - fail
// else - all on
void updateLED(int status) {
    if(status == 0) {
        digitalWrite(LED_RED_PIN, LOW);
        digitalWrite(LED_YELLOW_PIN, LOW);
        digitalWrite(LED_GREEN_PIN, LOW);
    } else if (status == 1) {
        digitalWrite(LED_RED_PIN, LOW);
        digitalWrite(LED_YELLOW_PIN, LOW);
        digitalWrite(LED_GREEN_PIN, HIGH);
    } else if (status == 2) {
        digitalWrite(LED_RED_PIN, LOW);
        digitalWrite(LED_YELLOW_PIN, HIGH);
        digitalWrite(LED_GREEN_PIN, LOW);
    } else if (status == 3) {
        digitalWrite(LED_RED_PIN, HIGH);
        digitalWrite(LED_YELLOW_PIN, LOW);
        digitalWrite(LED_GREEN_PIN, LOW);
    } else {
        digitalWrite(LED_RED_PIN, HIGH);
        digitalWrite(LED_YELLOW_PIN, HIGH);
        digitalWrite(LED_GREEN_PIN, HIGH);
    }


}