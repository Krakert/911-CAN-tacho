//
// Created by Stefan on 31/03/2024.
//

#ifndef INC_911_CAN_TACHO_TACHO_H
#define INC_911_CAN_TACHO_TACHO_H

#include "Arduino.h"
#include "../interpolation/interpolation.h"

#define MAX_PWM             255

// RPM limits
#define MAX_RPM             7000
#define MIN_RPM             1000
#define WIFI_RPM            9000

// RPM and corresponding multiplier values
const long rpms[] = {
        MIN_RPM,
        2000,
        3000,
        4000,
        5000,
        6000,
        MAX_RPM
};

const double multiplierTacho[] = {
        0.65,
        0.855,
        0.92,
        0.9,
        0.92,
        0.93,
        1.0
};

const size_t arraySizeRpm = sizeof(rpms) / sizeof(rpms[0]);

class Tacho {
public:
    explicit Tacho();

    void attach(int pin);

    void sweep() const;

    void update(long rpmIn) const;

    void writeRaw(uint8_t value) const;

private:

    uint8_t pin;

    virtual void setPin(int pin) { pinMode(pin, OUTPUT); }

};


#endif //INC_911_CAN_TACHO_TACHO_H
