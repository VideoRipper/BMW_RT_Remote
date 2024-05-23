#ifndef M5StampPicoMate_h
#define M5StampPicoMate_h

const char DeviceName[] = "M5 STAMP Pico Mate";

/* Undefine to disable monitoring of power supply */
#define _USEPOWERMONITOR
/* Undefine to disable the Neopixel status LED */
#define _USELED
/* Undefine to disable deep sleep */
//#define _USEDEEPSLEEP

#ifdef _USEPOWERMONITOR
#define POWER_EXT     32       // GPIO-pin that monitors the voltage of the power supply
#define POWER_INT     33       // GPIO-pin that monitors the voltage of the controller
#define POWER_MAX     5        // Maximum voltage to measure
#define POWER_DOWN    2048     // ADC level (0..4096) of external power at which the device should go to sleep
#define ADC_NUM       220000   // Voltage divider numerator value (actual used R value in Ohm)
#define ADC_DEN       194118   // Voltage divider denominator value (ideal R value in Ohm)
#endif

#ifdef _USELED
#define NUM_LEDS      1        // Number of LEDs connected (should be 1)
#define GPIO_LED      27       // GPIO-pin for the LED
#define TYPE_LED      NEO_GRBW + NEO_KHZ400
#endif

#define BUTTON_MUTE   18       // GPIO-pin for the Mute button
#define BUTTON_LEFT   19       // GPIO-pin for the Seek prior button
#define BUTTON_UP     21       // GPIO-pin for the Volume up button
#define BUTTON_DOWN   22       // GPIO-pin for the Volume down button
#define BUTTON_RIGHT  25       // GPIO-pin for the Seek next button

#define DEBOUNCE      50       // Button debounce value (in ms, default is 35)
#define HOLD_DELAY    1000     // The delay (in ms) to destinguish between short key press and long key press
#define BTN_PULLUP    true     // Use internal button I/O pullup (if set to false, you need to pull up the I/O's externally)
#define BTN_INVERT    true     // When true LOW is pressed, when false HIGH is pressed

#ifdef _USEDEEPSLEEP
#define SLEEP_DELAY   60       // Number of seconds before going to sleep after no connection
#endif

#endif
