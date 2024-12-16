#include <iostream>
#include <cstdint>
#include <vector>
#include <cmath>
#include <algorithm>

// Step 1: Replace 0 with -1 in the received vector
void replaceZeros(std::vector<int8_t> &w) {
    for (auto &bit : w) {
        bit = bit * 2 - 1; // Replace 0 with -1
    }
}

// Fast Hadamard Matrix product H ^ m
std::vector<std::vector<int8_t>> hadamardMatrix(int m) {
    int size = 1 << m; // 2^m
    std::vector<std::vector<int8_t>> H(size, std::vector<int8_t>(size, 1));

    for (int i = 1; i < size; i <<= 1) {
        for (int j = 0; j < i; ++j) {
            for (int k = 0; k < i; ++k) {
                H[j + i][k] = H[j][k];
                H[j][k + i] = H[j][k];
                H[j + i][k + i] = -H[j][k];
            }
        }
    }
    return H;
}

// Step 2: Multiply the received vector by the Hadamard matrix
void multiplyByHadamardMatrix(const std::vector<int8_t> &w, const std::vector<std::vector<int8_t>> &H, std::vector<int> &wm) {
    int n = w.size();

    // Matrix multiplication w * H = wm
    for (int j = 0; j < n; ++j) {
        int sum = 0;
        for (int i = 0; i < n; ++i) {
            sum += w[i] * H[j][i];
        }
        wm[j] = sum;
    }
}

// Step 3: Find the largest component of the vector
std::pair<int8_t, std::vector<uint8_t>> findLargestComponent(const std::vector<int> &wm, int m) {
    int maxPos = 0;
    int maxVal = std::abs(wm[0]);

    // Find the position j of the largest component of the vector
    for (size_t i = 1; i < wm.size(); ++i) {
        if (std::abs(wm[i]) > maxVal) {
            maxVal = std::abs(wm[i]);
            maxPos = i;
        }
    }

    // Decode the largest component
    std::vector<uint8_t> binaryIndex(m);
    for (int i = 0; i < m; ++i) {
        binaryIndex[i] = (maxPos >> i) & 1;
    }

    int bit = (wm[maxPos] > 0) ? 1 : 0; // Determine the sign of the largest component

    return {bit, binaryIndex};
}

// Decode a chunk of the received message
std::vector<uint8_t> decodeChunk(const std::vector<uint8_t> &chunk, int m, const std::vector<std::vector<int8_t>> &H) {
    // Convert chunk to integer vector and replace 0 with -1
    std::vector<int8_t> w(chunk.begin(), chunk.end());
    replaceZeros(w);

    std::vector<int> wm(w.size());
    multiplyByHadamardMatrix(w, H, wm);

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

    // Compute the Hadamard matrix once
    auto H = hadamardMatrix(m);

    // Split the received message into chunks of size n
    for (size_t i = 0; i < receivedMessage.size(); i += n) {
        std::vector<uint8_t> chunk(receivedMessage.begin() + i, receivedMessage.begin() + std::min(i + n, receivedMessage.size()));

        // Decode each chunk
        auto decodedChunk = decodeChunk(chunk, m, H);
        decodedMessage.insert(decodedMessage.end(), decodedChunk.begin(), decodedChunk.end());
    }

    return decodedMessage;
}