//
// Created by Stefan on 31/03/2024.
//

#include <tacho.h>

/**************************************************************************/
/**
 * @brief    Constructor for class
 */
/**************************************************************************/
Tacho::Tacho() : pin(0) {}


void Tacho::attach(int pin) {
    setPin(pin);
    this->pin = pin;
}

/**************************************************************************/
/**
 * @brief A simple boot-up sequences for the needle.
 */
/**************************************************************************/
bool Tacho::sweep() const {
    int i;
    // Sweep from 0 to 255
    for (i = 0; i <= 255; i++) {
        writeRaw(i);
        delay(3);
    }

    delay(300);

    // Sweep back from 255 to 0
    for (i = 255; i >= 0; i--) {
        writeRaw(i);
        delay(3);
    }
    delay(300);
    return true;
}

/**************************************************************************/
/**
 * @brief Calculate PWM value for the tachometer based on RPM and interpolation and clamp the value if needed.
 *
 * [Interpolation::interpolate()] translates the input to a new min - max range,
 * in this case we input a RPM value in the range 0 - [MAX_RPM + 100] and translate that to
 * a value in the range 0 - [MAX_PWM].
 *
 * [Interpolation::interpolate2d()] calculates a corresponding y value based on a x value,
 * in this case the y value is a multiplier, which is used to alter the PWM value from [Interpolation::interpolate()].
 */
/**************************************************************************/
void Tacho::update(long rpmIn) const {

    auto pwmValue = uint16_t(Interpolation::interpolate(rpmIn, 0, MAX_RPM + 100, 0, MAX_PWM) *
                             Interpolation::interpolate2d(rpmIn, rpms, multiplierTacho, arraySizeTacho));

    // Limit PWM value within the range of steps
    if (pwmValue > MAX_PWM) {
        analogWrite(pin, MAX_PWM);
    } else {
        analogWrite(pin, pwmValue);
    }
}

/**************************************************************************/
/**
 * @brief Simple write a PWM value
 *
 * @param value The values to write (0-255)
 */
/**************************************************************************/
void Tacho::writeRaw(uint8_t value) const {
    analogWrite(pin, value);
}


