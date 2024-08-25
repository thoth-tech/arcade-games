#pragma once

#include <cstdint>
#include <algorithm>
#include <stdexcept>
#include <limits>
#include <vector>

/**
 * @brief Simple random number generator based on XOR shift algorithm.
 */
struct XOR {
    /**
     * @brief Constructor for XOR random number generator.
     * @param initialSeed The initial seed value for the generator.
     */
    XOR(uint32_t initialSeed = 0x77777777);

    /**
     * @brief Generates a random integer in the range [min, max].
     * @param min The minimum value of the range.
     * @param max The maximum value of the range.
     * @return A random integer in the specified range.
     */
    inline int randomInt(int min, int max);

    /**
     * @brief Generates a random float in the range [min, max].
     * @param min The minimum value of the range.
     * @param max The maximum value of the range.
     * @return A random float in the specified range.
     */
    inline float randomFloat(float min = 0.0f, float max = 1.0f);

    /**
     * @brief Generates a boolean value with a given probability of being true.
     * @param probability The probability of returning true (0.0f to 1.0f).
     * @return true with the specified probability, false otherwise.
     */
    inline bool chance(float probability = 0.5f);

    /**
     * @brief Chooses a random element from a vector.
     * @param vec The vector from which to choose.
     * @return A random element from the vector.
     * @throws std::invalid_argument if the vector is empty.
     */
    template<typename T>
    inline T choose(const std::vector<T>& vec);

    /**
     * @brief Chooses a random element from an array.
     * @param arr The array from which to choose.
     * @return A random element from the array.
     */
    template<typename T, size_t N>
    inline T choose(const T (&arr)[N]);

private:
    /**
     * @brief Generates the next random number in the sequence.
     * @return The next random number.
     */
    inline uint32_t next();

    uint32_t seed; ///< The current seed value for the generator.
};

// Inline function definitions

inline XOR::XOR(uint32_t initialSeed) : seed(initialSeed) {}

inline uint32_t XOR::next() {
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}

inline int XOR::randomInt(int min, int max) {
    if (max < min) {
        std::swap(min, max);
    }
    uint32_t range = static_cast<uint32_t>(max - min + 1);
    return min + static_cast<int>(next() % range);
}

inline float XOR::randomFloat(float min, float max) {
    float normalized = next() * (1.0f / 4294967296.0f);
    return min + normalized * (max - min + std::numeric_limits<float>::epsilon());
}

inline bool XOR::chance(float probability) {
    return randomFloat() < probability;
}

template<typename T>
inline T XOR::choose(const std::vector<T>& vec) {
    if (vec.empty()) {
        throw std::invalid_argument("Vector is empty");
    }
    int index = randomInt(0, vec.size() - 1);
    return vec[index];
}

template<typename T, size_t N>
inline T XOR::choose(const T (&arr)[N]) {
    return arr[randomInt(0, N - 1)];
}
