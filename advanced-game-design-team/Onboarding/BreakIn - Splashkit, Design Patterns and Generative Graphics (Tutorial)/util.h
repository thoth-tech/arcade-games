#pragma once

#include "../../../games/splashkit mario/include/splashkit/splashkit.h"
#include "stdio.h"
#include <stdexcept>

/**
 * @brief Convert a hex string to a color.
 *
 * @param hex The hex string.
 * @return color The color.
 */
inline color color_from_hex(const std::string& hex) {
    int r, g, b;
    sscanf(hex.c_str(), "#%02x%02x%02x", &r, &g, &b);
    return rgb_color(r, g, b);
}

/**
 * @brief Map a value from one range to another.
 *
 * @param value The value to map.
 * @param input_min The input minimum.
 * @param input_max The input maximum.
 * @param output_min The output minimum.
 * @param output_max The output maximum.
 * @return double The mapped value.
 */
inline double map_value(double value, double input_min, double input_max, double output_min, double output_max) {
    // Calculate the ratio between the input range and output range
    double scale = (output_max - output_min) / (input_max - input_min);
    // Map the value to the new range
    return output_min + (value - input_min) * scale;
}

/**
 * @brief Clamp a value between a low and high value.
 *
 * @tparam T The value type.
 * @param value The value to clamp.
 * @param low The low value.
 * @param high The high value.
 * @return T The clamped value.
 */
template<typename T>
T clamp(const T& value, const T& low, const T& high);

template<typename T>
T clamp(const T& value, const T& low, const T& high) {
    return (value < low) ? low : (value > high) ? high : value;
}
