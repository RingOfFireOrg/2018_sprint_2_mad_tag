 #include "Blinker.h"

/*
* Intervals to wait between light and dark states
* declared static so that they use memory only once, even if multiple Blinker objects are created
*/
static const int LONG_BLINK = 2000;
static const int SHORT_BLINK = 1000;
static const int VERY_SHORT_BLINK = 250;
static const int VERY_VERY_SHORT_BLINK = 125;
// A look up table to convert between public speed constants and Speed_t namees
static const int blink_rate_table[Blinker::SPEED_ARRAY_SIZE] = {LONG_BLINK, SHORT_BLINK, VERY_SHORT_BLINK, VERY_VERY_SHORT_BLINK};


Blinker::Blinker(int LED_pin) {
    LED_pin_number = LED_pin;
}

Blinker::~Blinker() {
    LED_pin_number = 0;
}

/*
 * Call this from arduino "setup()"
 */
void Blinker::init() {
    pinMode(LED_pin_number, OUTPUT);
    enableLED();
}

/*
 * Calls this from arduino "loop()"
 * if this isn't called the LED won't blink.
 * But you can still turn it on and off
 */
void Blinker::service(Blinker::Speed_t blinkRate) {

    long current_time = millis();

    if (current_time - state_start_time > blink_rate_table[blinkRate]) {
        //toggle LED - built in LED is on an inverted signal
        if (lightOn) {
            disableLED();
        } else {
            enableLED();
        }
        state_start_time = millis();
    }
}

/*
 * Utility function to turn an LED on.
 * NOTE: The internal LED is on an inverted signal, so we have to 
 * treat it differently compared to an LED plugged into a DIO pin
 * (dont forget a current limiting resistor)
 */
void Blinker::enableLED() {
    if (LED_pin_number == LED_BUILTIN) {
        digitalWrite(LED_BUILTIN, LOW);
    } else {
        digitalWrite(LED_BUILTIN, HIGH);
    }
    lightOn = true;
}

/*
 * Utility function to turn an LED on.
 * NOTE: The internal LED is on an inverted signal, so we have to 
 * treat it differently compared to an LED plugged into a DIO pin
 * (dont forget a current limiting resistor)
 */
void Blinker::disableLED() {
    if (LED_pin_number == LED_BUILTIN) {
        digitalWrite(LED_BUILTIN, HIGH);
    } else {
        digitalWrite(LED_BUILTIN, LOW);
    }
    lightOn = false;
}

bool Blinker::isIlluminated() {
    return (lightOn);
}