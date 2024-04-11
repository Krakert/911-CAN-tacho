//
// Created by Stefan on 10/04/2024.
//

#include <blinker.h>

/**************************************************************************/
/**
 *  @brief    Constructor for class
 */
/**************************************************************************/
Blinker::Blinker() {
    debouncerLeft.attach(LEFT_PIN, INPUT_PULLUP);
    debouncerRight.attach(RIGHT_PIN, INPUT_PULLUP);

    debouncerLeft.interval(50);
    debouncerRight.interval(50);

    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
}

/**************************************************************************/
/**
 * @brief Blink the LED's as needed
 *
 * This method writes the correct values to the LED's.
 * The Rev limiter has priority over the blinkers
 */
/**************************************************************************/
void Blinker::blink() {
    if (blinkerStruct.rev) {
        timer(timerLimit);
        leds[0] = (timerLimit.ledState) ? CRGB::Red : CRGB::Black;
        leds[1] = (timerLimit.ledState) ? CRGB::Red : CRGB::Black;
    } else {
        leds[0] = (blinkerStruct.left) ? CRGB::Green : CRGB::Black;
        leds[1] = (blinkerStruct.right) ? CRGB::Green : CRGB::Black;
    }

    FastLED.show();
}

/**************************************************************************/
/**
 * @brief Update the state of the Rev bool in a stateBlinkerStruct
 *
 * @param state state to store
 */
/**************************************************************************/
void Blinker::setRevState(bool state) {
    blinkerStruct.rev = state;
}

/**************************************************************************/
/**
 * @brief Update the state of the GPIO
 *
 * This method read the GPIO pins with the help of the debouncer.
 * When the state has changed write the new state into the stateBlinkerStruct.
 */
/**************************************************************************/
void Blinker::readInputs() {
    debouncerLeft.update();
    debouncerRight.update();

    // Read the GPIO
    if (debouncerLeft.changed()) {
        blinkerStruct.left = !debouncerLeft.read();
    }

    if (debouncerRight.changed()) {
        blinkerStruct.right = !debouncerRight.read();
    }
}

/**************************************************************************/
/**
 * @brief Controls the blinking of an LED based on a timer.
 *
 * This method is responsible for toggling the state of an LED based on the specified interval.
 * It uses the millis() function to keep track of time and toggle the LED state accordingly.
 *
 * @param timerData A structure containing timer-related data.
 */
/**************************************************************************/
void Blinker::timer(timerStruct &timerData) {
    unsigned long currentMillis = millis();

    if (currentMillis - timerData.previousMillis >= timerData.interval) {
        // save the last time you blinked the LED
        timerData.previousMillis = currentMillis;

        if (timerData.ledState == LOW) {
            timerData.ledState = HIGH;
        } else {
            timerData.ledState = LOW;
        }
    }
}