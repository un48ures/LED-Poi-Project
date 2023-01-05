// Constants
const byte pipe_address[6] = "00001";
const int DEFAULT_BRIGHTNESS = 2; // General fix brightness - max brightness 255
const long interval_1 = 1000;     // ms 1st LED Standby blink interval
const long LED_red_on_time = 50;  // ms 1st LED Standby blink on-time
const int numberOfSlices = 150;

// LED Variables
CRGB leds[NUM_LEDS];
int ledState = LOW;
volatile byte fadectr = 0;
String color = "red";
uint8_t gHue = 128;

// Message Variables
int message_global = 99; // default message 99 -> Standby RED LED blinking
long message_brightness = 0;

// Additional
int looping = 0;
bool fillupDone = 0;
bool filldownDone = 0;
unsigned long previousMillis = 0;
// int ledPin = 13; = SCK PIN bei Teensy 4.0
bool firstStart = true;
int LEDstate = 0;
bool blink_once = 0;
int time_on = 0; // help variable for blinking

// PULSE FADE Variablen
int hue = 0;        // Color
int saturation = 0; // saturation
int value = 0;      // brightness value
