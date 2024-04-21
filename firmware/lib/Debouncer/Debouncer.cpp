//
// Created by Stefan on 10/04/2024.
//

#include "Debouncer.h"

Debouncer::Debouncer() : pin(0) {}

void Debouncer::interval(uint16_t interval_millis) {
    this->interval_millis = interval_millis;
}

void Debouncer::begin() {
    state = 0;
    if (readCurrentState()) {
        setStateFlag(DEBOUNCED_STATE | UNSTABLE_STATE);
    }
    previous_millis = millis();
}

void Debouncer::attach(int pin) {
    this->pin = pin;
    begin();
}

void Debouncer::attach(int pin, int mode) {
    setPinMode(pin, mode);
    this->attach(pin);
}

bool Debouncer::update() {
    unsetStateFlag(CHANGED_STATE);
    // Read the state of the switch in a temporary variable.
    bool currentState = readCurrentState();


    if (currentState != getStateFlag(UNSTABLE_STATE)) {
        previous_millis = millis();
        toggleStateFlag(UNSTABLE_STATE);
    } else if (millis() - previous_millis >= interval_millis) {
        if (currentState != getStateFlag(DEBOUNCED_STATE)) {
            previous_millis = millis();
            changeState();
        }
    }
    return changed();
}

bool Debouncer::read() {
    return getStateFlag(DEBOUNCED_STATE);
}


inline void Debouncer::changeState() {
    toggleStateFlag(DEBOUNCED_STATE);
    setStateFlag(CHANGED_STATE);
    durationOfPreviousState = millis() - stateChangeLastTime;
    stateChangeLastTime = millis();
}