/*


Code for motor controls


*/

//motor controller
static const int MTR_STEP_PIN = 13;
static const int MTR_DIR_PIN = 14;
static const int MTR_ENA_PIN = 15;

void initMotor();
void motorClockwise(int onTime, int dutyCycle);
void motorCounterClockwise(int onTime, int dutyCycle);