#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <bitset>
#include <thread>
#include <mutex>

// Step 1: Replace 0 with -1 in the received vector
void replaceZeros(std::vector<int> &w) {
    for (auto &bit : w) {
        if (bit == 0) {
            bit = -1;
        }
    }
}

// Fast Hadamard Matrix product H ^ m
std::vector<std::vector<int>> hadamardMatrix(int m) {
    int size = 1 << m; // 2^m
    std::vector<std::vector<int>> H(size, std::vector<int>(size, 1));

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
std::vector<int> multiplyByHadamardMatrix(const std::vector<int> &w, const std::vector<std::vector<int>> &H) {
    int n = w.size();
    std::vector<int> wm(n, 0);

    // Matrix multiplication w * H = wm
    for (int j = 0; j < n; ++j) {
        int sum = 0;
        for (int i = 0; i < n; ++i) {
            sum += w[i] * H[j][i];
        }
        wm[j] = sum;
    }
    return wm;
}

// Step 3: Find the largest component of the vector
std::pair<int, std::vector<bool>> findLargestComponent(const std::vector<int> &wm, int m) {
    int maxPos = 0;
    int maxVal = abs(wm[0]);

    // Find the position j of the largest component of the vector
    for (std::vector<int>::size_type i = 1; i < wm.size(); ++i) {
        if (abs(wm[i]) > maxVal) {
            maxVal = abs(wm[i]);
            maxPos = i;
        }
    }

    // Decode the largest component
    std::vector<bool> binaryIndex(m);
    for (int i = 0; i < m; ++i) {
        binaryIndex[i] = (maxPos >> i) & 1;
    }

    int bit = (wm[maxPos] > 0) ? 1 : 0; // Determine the sign of the largest component

    return {bit, binaryIndex};
}

std::vector<bool> decodeChunk(const std::vector<bool> &chunk, int m, const std::vector<std::vector<int>> &H) {
    // Convert chunk to integer vector and replace 0 with -1
    std::vector<int> w(chunk.begin(), chunk.end());
    replaceZeros(w);

    auto wm = multiplyByHadamardMatrix(w, H);
    auto [bit, index] = findLargestComponent(wm, m);

    // Construct the decoded message
    std::vector<bool> decodedMessage;
    decodedMessage.push_back(bit);
    for (int i = 0; i < m; ++i) {
        decodedMessage.push_back(index[i]);
    }

    return decodedMessage;
}

std::vector<bool> decode(const std::vector<bool> &receivedMessage, int m) {
    int n = 1 << m; // 2^m
    std::vector<bool> decodedMessage;

    // Compute the Hadamard matrix once
    auto H = hadamardMatrix(m);

    // size_t numChunks = receivedMessage.size() / n;

    // Split the received message into chunks of size n
    for (size_t i = 0; i < receivedMessage.size(); i += n) {
        std::vector<bool> chunk(receivedMessage.begin() + i, receivedMessage.begin() + std::min(i + n, receivedMessage.size()));

        // Decode each chunk
        auto decodedChunk = decodeChunk(chunk, m, H);
        decodedMessage.insert(decodedMessage.end(), decodedChunk.begin(), decodedChunk.end());
    }

    return decodedMessage;
}