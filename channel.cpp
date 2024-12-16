#include <vector>
#include <random>
#include <ctime>
#include <cstdint>

// Function to introduce errors into the encoded bit array
std::vector<uint8_t> introduceErrors(const std::vector<uint8_t>& encodedMessage, float q) {
    std::vector<uint8_t> corruptedMessage = encodedMessage;
    std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr))); // Random number generator
    std::uniform_real_distribution<float> distribution(0.0, 1.0); // Uniform distribution between 0 and 1

    for (size_t i = 0; i < corruptedMessage.size(); ++i) {
        if (distribution(generator) < q) {
            corruptedMessage[i] = !corruptedMessage[i]; // Flip the bit
        }
    }

    return corruptedMessage;
}