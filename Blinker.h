#ifndef BLINKER_H_INCLUDED
#define BLINKER_H_INCLUDED

#include <Arduino.h>
#include <time.h>

class Blinker {
    public:
        /*
         * This names a set of "speeds" that we can ask the Blinker object to blink at.
         * These speeds are just "names" of speeds. The actual rates are in the 
         * Blinker.cpp file.
         */
        enum Speed_t {SLOW, FAST, HYPER, SUPERHYPER, SPEED_ARRAY_SIZE};

        /*
         * This is a constructor. It is the function for making a new 
         * Blinker object. It requires that you give it a pin LED_pin_number
         * or it will default to the builtin LED. You can also explicitly 
         * give it the number for the builtin LED
         */
        Blinker(int LED_pin = LED_BUILTIN);

        /*
         * a c++ destructor. you will never call this. the compiler takes care
         * of that for you, when it needs to happen, assuming you do other things
         * correctly.
         */
        virtual ~Blinker();

        /*
         * Call this from arduino "setup()" to so the Blinker can set the pins up
         * at the right time.
         */
        virtual void init();

        /*
         * Calls this from arduino "loop()"
         * If you don't call this function regularly, then the LED won't blink.
         * You'll still be able to call enableLED() and disableLED(), but you'll
         * have to track when they come on and off.
         */
        virtual void service(Speed_t blinkRate);

        /* 
         * turn LED on
         */
        void enableLED();

        /* 
         * turn LED off
         */
        void disableLED();
        
        /* 
         * find out the state of the LED
         */
        bool isIlluminated();

    private:
        int LED_pin_number;
        bool lightOn = false;
        long state_start_time = 0;
};

#endif // BLINKER_H_INCLUDED