//
// Created by Stefan on 10/04/2024.
//

#include <blinker.h>

/**************************************************************************/
/*!
  @brief    Constructor for class
*/
/**************************************************************************/
Blinker::Blinker() {
    debouncerLeft.attach(LEFT_PIN, INPUT_PULLUP);
    debouncerRight.attach(RIGHT_PIN, INPUT_PULLUP);

    debouncerLeft.interval(50);
    debouncerRight.interval(50);

    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
}

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

void Blinker::setRevState(bool state) {
    blinkerStruct.rev = state;
}


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