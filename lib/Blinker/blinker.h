//
// Created by Stefan on 10/04/2024.
//

#ifndef INC_911_CAN_TACHO_BLINKER_H
#define INC_911_CAN_TACHO_BLINKER_H

#include <Arduino.h>
#include <FastLED.h>
#include <Debouncer.h>

#define LED_PIN             D2
#define LEFT_PIN            D3
#define RIGHT_PIN           D4
#define NUM_LEDS            2

#define BLINK_TIME_MS       150

#define GPIO_READ(pin) ((GPI & (1 << pin)) >> pin)

typedef struct {
    bool left;
    bool right;
    bool rev;
} stateBlinkerStruct;

typedef struct {
    unsigned long previousMillis;
    unsigned long interval;
    bool ledState;
} timerStruct;

class Blinker {

public:
    Blinker();

    void blink();

    void readInputs();

    void setRevState(bool state);

private:

    void timer(timerStruct &timerData);

    CRGB leds[NUM_LEDS]{};
    stateBlinkerStruct blinkerStruct{};
    Debouncer debouncerLeft;
    Debouncer debouncerRight;

    timerStruct timerLimit{0, BLINK_TIME_MS, LOW};

};

#endif //INC_911_CAN_TACHO_BLINKER_H
