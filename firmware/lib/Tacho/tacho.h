//
// Created by Stefan on 31/03/2024.
//

#ifndef INC_911_CAN_TACHO_TACHO_H
#define INC_911_CAN_TACHO_TACHO_H

#include "Arduino.h"
#include "../interpolation/interpolation.h"
#include "../../src/config.h"

#define MAX_PWM             255

/**
 * @brief setup the tacho correction multiplier
 * 
 *    multiplier
 *  1.0   |                            x
 *  0.9   |            x   x   x   x   
 *  0.8   |        x   
 *  0.7   |    
 *  0.6   |    x   
 *  0.5   |    
 *       ------------------------------
 *        0   1   2   3   4   5   6   7 RPM
 *   
 */
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

const uint8_t arraySizeTacho = sizeof(rpms) / sizeof(rpms[0]);

class Tacho {
public:
    explicit Tacho();

    void attach(int pin);

    bool sweep() const;

    void update(long rpmIn) const;

    void writeRaw(uint8_t value) const;

private:

    uint8_t pin;

    virtual void setPin(int pin) { pinMode(pin, OUTPUT); }

};


#endif //INC_911_CAN_TACHO_TACHO_H
