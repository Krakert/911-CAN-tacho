//
// Created by Stefan on 10/04/2024.
//

#ifndef INC_911_CAN_TACHO_DEBOUNCER_H
#define INC_911_CAN_TACHO_DEBOUNCER_H

#include "Arduino.h"

#define GPIO_READ(pin) ((GPI & (1 << pin)) >> pin)
#define DEBOUNCED_STATE     0b00000001 // Final returned calculated debounced state
#define UNSTABLE_STATE      0b00000010 // Actual last state value behind the scene
#define CHANGED_STATE       0b00000100 // The DEBOUNCED_STATE has changed since last update()

class Debouncer {

private:
    uint16_t interval_millis;
    uint8_t state;
    uint8_t pin;

    unsigned long previous_millis;
    unsigned long stateChangeLastTime;
    unsigned long durationOfPreviousState;

    void begin();

    void changeState();

    void setStateFlag(const uint8_t flag) { state |= flag; }

    void unsetStateFlag(const uint8_t flag) { state &= ~flag; }

    void toggleStateFlag(const uint8_t flag) { state ^= flag; }

    bool getStateFlag(const uint8_t flag) { return ((state & flag) != 0); }

    virtual bool readCurrentState() { return GPIO_READ(pin); }

    virtual void setPinMode(int pin, int mode) { pinMode(pin, mode); }

public:
    Debouncer();

    void interval(uint16_t interval_millis);

    bool update();

    bool read();

    bool changed() { return getStateFlag(CHANGED_STATE); }

    void attach(int pin, int mode);

    void attach(int pin);
};

#endif //INC_911_CAN_TACHO_DEBOUNCER_H
