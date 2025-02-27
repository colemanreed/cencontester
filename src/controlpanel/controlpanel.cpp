/*


Code for control panel


*/

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

void initLCD();
void initIO(); // for buttons and LEDs