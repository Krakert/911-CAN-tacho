//
// Created by Stefan on 11/04/2024.
//

#ifndef INC_911_CAN_TACHO_INTERPOLATION_H
#define INC_911_CAN_TACHO_INTERPOLATION_H

#include "Arduino.h"

class Interpolation {
public:

    /**
     * @brief Interpolates a value from one range to another.
     *
     * This function takes a value and maps it from one range (defined by in_min and in_max)
     * to another range (defined by out_min and out_max) using linear interpolation.
     *
     * @param value The value to be interpolated.
     * @param in_min The minimum value of the input range.
     * @param in_max The maximum value of the input range.
     * @param out_min The minimum value of the output range.
     * @param out_max The maximum value of the output range.
     * @return The interpolated value.
     */
    static double interpolate(double value, double in_min, double in_max, double out_min, double out_max) {
        return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    /**
     * @brief Performs 2D interpolation using linear interpolation.
     *
     * This function takes a value and performs 2D interpolation based on the provided arrays of x and y values.
     * It first checks if the given value is outside the range of the provided x values and returns the corresponding y value
     * if it is. Then, it finds the appropriate interval in the x array and performs linear interpolation within that interval
     * to determine the corresponding y value.
     *
     * @param value The value to be interpolated.
     * @param x An array of x values representing the independent variable.
     * @param y An array of y values representing the dependent variable.
     * @param size The size of the x and y arrays.
     * @return The interpolated y value corresponding to the given x value.
     */
    template<typename Tx, typename Ty>
    static double interpolate2d(long value, const Tx x[], const Ty y[], uint8_t size) {
        // Check if RPM is below the lowest RPM in the table
        if (value <= x[0]) {
            return y[0];
        }

        // Check if RPM is above the highest RPM in the table
        if (value >= x[size - 1]) {
            return y[size - 1];
        }

        // Find the last index less than the searched value
        uint8_t idx;
        for (idx = 0; idx < size - 1; idx++) {
            if (x[idx + 1] > value) {
                break;
            }
        }
        // Perform linear interpolation
        return interpolate(value, x[idx], x[idx + 1], y[idx], y[idx + 1]);
    }

};

#endif //INC_911_CAN_TACHO_INTERPOLATION_H
