#include <vector>
#include <cstdint>
#include <iostream>

// Recursive encoding function for Reed-Muller RM(1, m)
void encodeRecursive(const std::vector<uint8_t>& binaryMessage, size_t m, std::vector<uint8_t>& encodedMessage, size_t offset, size_t stride) {
    if (m == 0) {
        // Base case: Constant bit
        encodedMessage[offset] = binaryMessage[0];
        return;
    }

    size_t halfStride = stride / 2;

    // Encode the first half using the first m-1 bits
    encodeRecursive(binaryMessage, m - 1, encodedMessage, offset, halfStride);

    // Encode the second half by XORing the first half with the next bit
    for (size_t i = 0; i < halfStride; ++i) {
        encodedMessage[offset + halfStride + i] = 
            encodedMessage[offset + i] ^ binaryMessage[m];
    }
}

// Top-level encoding function
std::vector<uint8_t> encodeMessage(const std::vector<uint8_t>& binaryMessage, size_t m) {
    size_t n = 1 << m; // n = 2^m
    size_t messageBitsPerBlock = m + 1;

    // Calculate the total number of blocks, padding the last block if necessary
    size_t fullBlocks = binaryMessage.size() / messageBitsPerBlock;
    size_t remainder = binaryMessage.size() % messageBitsPerBlock;
    size_t totalBlocks = fullBlocks + (remainder > 0 ? 1 : 0);

    // Allocate the encoded message
    std::vector<uint8_t> encodedMessage(totalBlocks * n, 0);

    // Process each block
    for (size_t block = 0; block < totalBlocks; ++block) {
        std::vector<uint8_t> messageBlock(m + 1, 0);
        // Copy the block's message bits, padding with 0 if necessary
        for (size_t i = 0; i < m + 1; ++i) {
            size_t bitIndex = block * messageBitsPerBlock + i;
            if (bitIndex < binaryMessage.size()) {
                messageBlock[i] = binaryMessage[bitIndex];
            }
        }

        // Encode the block recursively
        encodeRecursive(messageBlock, m, encodedMessage, block * n, n);
    }

    return encodedMessage;
}