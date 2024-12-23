#include <vector>
#include <cmath>
#include <iostream>
#include "decoder.h"
#include <cstdint>
#include <algorithm>
#include <chrono>

// Iterative Fast Hadamard Transform
void fastHadamardTransforms(std::vector<int>& vec) {
    const size_t N = vec.size();
    for (size_t h = 1; h < N; h *= 2) {
        for (size_t i = 0; i < N; i += 2 * h) {
            for (size_t j = i; j < i + h; ++j) {
                int x = vec[j];
                int y = vec[j + h];
                vec[j] = x + y;
                vec[j + h] = x - y;
            }
        }
    }
}

// Optimized decode chunks using bit manipulation
std::vector<uint8_t> decodeChunks(const std::vector<uint8_t>& message, int m) {
    const size_t N = 1 << m;
    std::vector<int> vector(N);
    
    // Vectorized conversion from uint8_t to int (-1/1)
    for (size_t i = 0; i < N; ++i) {
        vector[i] = message[i] ? 1 : -1;
    }

    fastHadamardTransforms(vector);
    
    // Find largest absolute value using single pass
    int maxAbsValue = 0;
    size_t position = 0;
    for (size_t i = 0; i < N; ++i) {
        const int absValue = std::abs(vector[i]);
        if (absValue > maxAbsValue) {
            maxAbsValue = absValue;
            position = i;
        }
    }
    
    // Prepare result with pre-allocated size
    std::vector<uint8_t> result(m + 1);
    result[0] = vector[position] >= 0 ? 1 : 0;
    
    // Use bit operations for position conversion
    for (int i = 0; i < m; ++i) {
        result[i + 1] = (position >> i) & 1;
    }
    
    return result;
}

// Optimized message splitting with pre-allocated memory
std::vector<std::vector<uint8_t>> splitMessageForDecoding(const std::vector<uint8_t>& message, size_t chunkSize) {
    const size_t numChunks = (message.size() + chunkSize - 1) / chunkSize;
    std::vector<std::vector<uint8_t>> chunks;
    chunks.reserve(numChunks);
    
    for (size_t i = 0; i < message.size(); i += chunkSize) {
        chunks.emplace_back(
            message.begin() + i,
            message.begin() + std::min(message.size(), i + chunkSize)
        );
        if (chunks.back().size() < chunkSize) {
            chunks.back().resize(chunkSize, 0);
        }
    }
    return chunks;
}

// Main decode function with optimizations
std::vector<uint8_t> decode(const std::vector<uint8_t>& message, int m) {
    const size_t chunkSize = 1 << m;
    const auto chunks = splitMessageForDecoding(message, chunkSize);
    const size_t resultSize = chunks.size() * (m + 1);
    
    std::vector<uint8_t> decoded;
    decoded.reserve(resultSize);
    
    for (const auto& chunk : chunks) {
        auto decodedChunk = decodeChunks(chunk, m);
        decoded.insert(decoded.end(), decodedChunk.begin(), decodedChunk.end());
    }
    
    return decoded;
}