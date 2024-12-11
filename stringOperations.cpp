#include <bitset>
#include <vector>
#include "stringOperations.h"

// Convert a string to a binary vector
std::vector<bool> stringToBinary(const std::string& text) {
    std::vector<bool> binaryMessage;
    for (char c : text) {
        std::bitset<8> binary(c);
        for (int j = 0; j < 8; j++) {
            binaryMessage.push_back(binary[j]);
        }
    }
    return binaryMessage;
}

// Convert a binary vector back to a string
std::string binaryToString(const std::vector<bool>& binaryMessage) {
    std::string text;
    std::vector<bool> paddedBinaryMessage = binaryMessage;
    
    // Padding to make the size a multiple of 8
    while (paddedBinaryMessage.size() % 8 != 0) {
        paddedBinaryMessage.push_back(0); // Add padding zeros
    }

    for (size_t i = 0; i < paddedBinaryMessage.size(); i += 8) {
        std::bitset<8> byte;
        for (size_t j = 0; j < 8; j++) {
            byte[j] = paddedBinaryMessage[i + j];
        }
        text += static_cast<char>(byte.to_ulong());
    }
    return text;
}