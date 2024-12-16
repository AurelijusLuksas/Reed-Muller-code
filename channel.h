#ifndef CHANNEL_H
#define CHANNEL_H

#include <vector>

// Function to introduce errors into the encoded bit array
std::vector<uint8_t> introduceErrors(const std::vector<uint8_t>& encodedMessage, float q);

#endif // CHANNEL_H