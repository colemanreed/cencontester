/*


Code for solenoids (button pusher)


*/

//button pusher
static const int SOLENOID0_PIN = 10; 
static const int SOLENOID2_PIN = 11; 
static const int SOLENOID5_PIN = 12; 

void initSolenoids();
void extendSolenoid0(); // solenoid for keypad button '0'
void extendSolenoid2(); // solenoid for keypad button '0'
void extendSolenoid5(); // solenoid for keypad button '0'