/*


Code for control panel


*/

#include "controlpanel.h"

//button pusher
static const int LCD_RST_PIN = 16;
static const int LCD_ENA_PIN = 17;
static const int LCD_D4_PIN = 19;
static const int LCD_D5_PIN = 20;
static const int LCD_D6_PIN = 35;
static const int LCD_D7_PIN = 36;

static const int CYCLE_UP_PIN = 13;
static const int CYCLE_DOWN_PIN = 14;

static const int LED_RED_PIN = 15;
static const int LED_YELLOW_PIN = 16;
static const int LED_GREEN_PIN = 17;

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

LiquidCrystal lcd;

void initControlPanel() {
    lcd = LiquidCrystal(LCD_RST_PIN, LCD_ENA_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);

    lcd.print("Number of cycles "); // Print a message to the LCD

    // Initialize the pushbutton pins as inputs
    pinMode(CYCLE_UP_PIN, INPUT);
    pinMode(CYCLE_DOWN_PIN, INPUT);

    // Display the initial cycle value
    lcd.setCursor(0, 1);
    lcd.print(cycleSet);
}

int getUserCycles() {
    return cycleSet;
}

// Function to update the LCD
void updateLCD() {
    lcd.setCursor(0, 1);  // Position to display the count
    lcd.print("                "); // Clear the line
    lcd.setCursor(0, 1);
    lcd.print(cycleSet);   // Display the updated count
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
        updateLCD();
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
        updateLCD();
        }
        downbuttonState = currentDownButtonState;
    }

    // Save the last button states for the next loop
    lastUpbuttonState = currentUpButtonState;
    lastDownbuttonState = currentDownButtonState;

}


