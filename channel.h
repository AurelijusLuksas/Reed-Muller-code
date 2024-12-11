#ifndef CHANNEL_H
#define CHANNEL_H

#include <vector>

// Function to introduce errors into the encoded bit array
std::vector<bool> introduceErrors(const std::vector<bool>& encodedMessage, float q);

#endif // CHANNEL_H