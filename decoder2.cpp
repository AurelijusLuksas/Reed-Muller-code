#include <iostream>
#include <cstdint>
#include <vector>
#include <cmath>
#include <algorithm>

// Step 1: Replace 0 with -1 in the received vector
void replaceZeros(std::vector<int8_t> &w) {
    for (auto &bit : w) {
        bit = (bit == 0) ? -1 : bit; // Replace 0 with -1
    }
}

// Fast Hadamard Transform (in-place)
void hadamardTransform(std::vector<int>& vec) {
    int n = vec.size();
    for (int len = 1; len < n; len *= 2) {
        for (int i = 0; i < n; i += 2 * len) {
            for (int j = 0; j < len; ++j) {
                int a = vec[i + j];
                int b = vec[i + j + len];
                vec[i + j] = a + b;
                vec[i + j + len] = a - b;
            }
        }
    }
}

// Step 2: Multiply the received vector by the Hadamard transform
std::vector<int> applyHadamardTransform(const std::vector<int8_t>& w) {
    // Convert w to an integer vector for arithmetic operations
    std::vector<int> wm(w.begin(), w.end());

    // Perform the in-place Hadamard transform
    hadamardTransform(wm);

    return wm;
}

// Step 3: Find the largest component of the vector
std::pair<int8_t, std::vector<uint8_t>> findLargestComponent(const std::vector<int>& wm, int m) {
    auto maxIt = std::max_element(wm.begin(), wm.end(), [](int a, int b) {
        return std::abs(a) < std::abs(b);
    });

    int maxPos = std::distance(wm.begin(), maxIt);
    int bit = (*maxIt > 0) ? 1 : 0;

    std::vector<uint8_t> binaryIndex(m);
    for (int i = 0; i < m; ++i) {
        binaryIndex[i] = (maxPos >> i) & 1;
    }

    return {bit, binaryIndex};
}

// Decode a chunk of the received message
std::vector<uint8_t> decodeChunk(const std::vector<uint8_t> &chunk, int m) {
    // Convert chunk to integer vector and replace 0 with -1
    std::vector<int8_t> w(chunk.begin(), chunk.end());
    replaceZeros(w);

    std::vector<int> wm = applyHadamardTransform(w);

    auto [bit, index] = findLargestComponent(wm, m);

    // Construct the decoded message
    std::vector<uint8_t> decodedMessage;
    decodedMessage.push_back(bit);
    decodedMessage.insert(decodedMessage.end(), index.begin(), index.end());
    
    return decodedMessage;
}

// Decode the entire received message
std::vector<uint8_t> decode(const std::vector<uint8_t> &receivedMessage, int m) {
    int n = 1 << m; // 2^m
    std::vector<uint8_t> decodedMessage;

    // Split the received message into chunks of size n
    for (size_t i = 0; i < receivedMessage.size(); i += n) {
        std::vector<uint8_t> chunk(receivedMessage.begin() + i, receivedMessage.begin() + std::min(i + n, receivedMessage.size()));

        // Decode each chunk
        auto decodedChunk = decodeChunk(chunk, m);
        decodedMessage.insert(decodedMessage.end(), decodedChunk.begin(), decodedChunk.end());
    }

    return decodedMessage;
}