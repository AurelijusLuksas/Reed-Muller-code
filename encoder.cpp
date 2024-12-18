#include <vector>
#include "encoder.h"
#include <iostream>
#include <cstdint>

// Generate the Reed-Muller generator matrix for RM(1, m)
void generateReedMullerMatrix(int m, std::vector<std::vector<uint8_t>>& matrix, size_t n) {

    for (size_t i = 0; i < n; i++) {
        matrix[i][0] = 1; // Set the constant term (first column) to 1
        for (int j = 1; j <= m; j++) {
            // Set the j-th column based on the binary representation of i
            matrix[i][j] = (i >> (j - 1)) & 1;
        }
    }
}

// Encode the binary message using Reed-Muller RM(1, m)
std::vector<uint8_t> encodeMessage(const std::vector<uint8_t>& binaryMessage, int m) {
    int n = 1 << m; // n = 2^m
    std::vector<std::vector<uint8_t>> reedMullerMatrix(n, std::vector<uint8_t>(m + 1, 0)); // Initialize the matrix with zeros
    generateReedMullerMatrix(m, reedMullerMatrix, n);
    std::vector<uint8_t> encodedMessage;
    encodedMessage.reserve(binaryMessage.size() * n / (m + 1));

    for (size_t i = 0; i < binaryMessage.size(); i += m + 1) {
        std::vector<uint8_t> encodedPart(n, 0); // Reused buffer for each part
        // Encode directly without needing a separate 'part' vector
        for (int j = 0; j < n; j++) {
            for (int k = 0; k <= m; k++) {
                size_t idx = i + k;
                uint8_t bit = (idx < binaryMessage.size()) ? binaryMessage[idx] : 0; // Implicit padding
                encodedPart[j] ^= (reedMullerMatrix[j][k] & bit);
            }
        }

        encodedMessage.insert(encodedMessage.end(), encodedPart.begin(), encodedPart.end());
    }

    return encodedMessage;
}

