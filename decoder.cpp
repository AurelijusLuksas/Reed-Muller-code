#include <vector>
#include <cmath>
#include <iostream>
#include "decoder.h"
#include <cstdint>
#include <algorithm>
#include <chrono>

// Recursive Fast Hadamard Transform
void fastHadamardTransformsRecursive(std::vector<int>& vec, size_t start, size_t end) {
    if (end - start == 1) return; // Base case: single element

    size_t mid = start + (end - start) / 2;
    fastHadamardTransformsRecursive(vec, start, mid); // first half
    fastHadamardTransformsRecursive(vec, mid, end);   // second half

    for (size_t i = start; i < mid; ++i) {
        int a = vec[i];
        int b = vec[i + (mid - start)];
        vec[i] = a + b;               // (sum)
        vec[i + (mid - start)] = a - b; // (difference)
    }
}

// Fast Hadamard Transform (driver)
std::vector<int> fastHadamardTransforms(const std::vector<uint8_t>& message, int m) {
    size_t N = 1 << m; // (2^m)
    std::vector<int> vector(message.size());
    for (size_t i = 0; i < message.size(); ++i) {
        vector[i] = message[i] ? 1 : -1;
    }

    fastHadamardTransformsRecursive(vector, 0, N);

    return vector;
}

std::vector<uint8_t> reverseVector(const std::vector<uint8_t>& vector) {
    std::vector<uint8_t> reversed = vector;
    std::reverse(reversed.begin(), reversed.end());
    return reversed;
}

std::vector<uint8_t> decodeChunks(std::vector<uint8_t> message, int m) {
    std::vector<int> transformedMessage = fastHadamardTransforms(message, m);
    
    // Find the largest component position and sign
    int largestValue = 0;
    int position = 0;
    for (size_t i = 0; i < transformedMessage.size(); ++i) {
        int absValue = transformedMessage[i] < 0 ? -transformedMessage[i] : transformedMessage[i];
        if (absValue > largestValue) {
            largestValue = absValue;
            position = i;
        }
    }
    int sign = transformedMessage[position] >= 0 ? 1 : 0;

    // Convert position to binary and reverse it
    std::vector<uint8_t> positionInBits(m, 0);
    for (int i = 0; i < m; ++i) {
        positionInBits[i] = (position >> i) & 1;
    }

    // Add the sign bit to the result
    positionInBits.insert(positionInBits.begin(), sign);

    return positionInBits;
}

std::vector<std::vector<uint8_t>> splitMessageForDecoding(const std::vector<uint8_t>& message, size_t chunkSize) {
    std::vector<std::vector<uint8_t>> chunks;
    for (size_t i = 0; i < message.size(); i += chunkSize) {
        std::vector<uint8_t> chunk(message.begin() + i, message.begin() + std::min(message.size(), i + chunkSize));
        chunk.resize(chunkSize, 0);
        chunks.push_back(chunk);
    }
    return chunks;
}

std::vector<uint8_t> decode(std::vector<uint8_t> message, int m) {
    std::vector<uint8_t> decoded;
    decoded.reserve(message.size());

    auto chunks = splitMessageForDecoding(message, 1 << m);

    for (const auto& chunk : chunks) {
        // auto start = std::chrono::high_resolution_clock::now();
        auto decodedChunk = decodeChunks(chunk, m);
        // auto end = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<double> elapsed = end - start;
        // std::cout << "Time taken to decode chunk: " << elapsed.count() << " seconds" << std::endl;
        decoded.insert(decoded.end(), decodedChunk.begin(), decodedChunk.end());
    }
    return decoded;
}