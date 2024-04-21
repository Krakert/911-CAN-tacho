//
// Created by Stefan on 10/04/2024.
//

#ifndef INC_911_CAN_TACHO_BLINKER_H
#define INC_911_CAN_TACHO_BLINKER_H

#include <Arduino.h>
#include <FastLED.h>
#include <Debouncer.h>
#include <interpolation.h>
#include "../../src/config.h"

#define LED_PIN             D2
#define LEFT_PIN            D3
#define RIGHT_PIN           D4


#define GPIO_READ(pin) ((GPI & (1 << pin)) >> pin)
/**
 * @brief setup the RPM limit based on CLT
 * 
 *       RPM x100
 *  7   |                 x
 *  6   |                 
 *  5   |        x
 *  4   |x   
 *  3   |
 *  2   |
 *       --------------------
 *       0    20    40    60 Temp
 *   
 */

const long rpmLimit[] = {
        4000,
        4500,
        5000,
        MAX_RPM
};

const long tempRange[] = {
        0,
        20,
        40,
        60
};

const uint8_t arraySizeSoftLimit = sizeof(rpmLimit) / sizeof(rpmLimit[0]);

typedef struct {
    bool left;
    bool right;
    bool revHard;
    bool revSoft;
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

    void setRevState(int engineTemp, uint16_t rpm);

    void test(bool disable = false);

    CRGB leds[2];

private:

    static void timer(timerStruct &timerData);

    stateBlinkerStruct blinkerStruct{};
    Debouncer debouncerLeft;
    Debouncer debouncerRight;

    timerStruct timerLimitHard{0, BLINK_TIME_HARD_MS, LOW};
    timerStruct timerLimitSoft{0, BLINK_TIME_SOFT_MS, LOW};

};

#endif //INC_911_CAN_TACHO_BLINKER_H
