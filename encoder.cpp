#include <vector>
#include "encoder.h"
#include <iostream>
#include <cstdint>

// Generate the Reed-Muller generator matrix for RM(1, m)
std::vector<std::vector<uint8_t>> generateReedMullerMatrix(int m) {
    int n = 1 << m; // n = 2^m, the number of columns in the generator matrix
    std::vector<std::vector<uint8_t>> matrix(n, std::vector<uint8_t>(m + 1, 0)); // Initialize the matrix with zeros

    for (int i = 0; i < n; i++) {
        matrix[i][0] = 1; // Set the constant term (first column) to 1
        for (int j = 1; j <= m; j++) {
            // Set the j-th column based on the binary representation of i
            matrix[i][j] = (i & (1 << (j - 1))) ? 1 : 0;
        }
    }
    return matrix; // Return the generated matrix
}

// Encode the binary message using Reed-Muller RM(1, m)
std::vector<uint8_t> encodeMessage(const std::vector<uint8_t>& binaryMessage, int m) {
    int n = 1 << m; // n = 2^m
    std::vector<std::vector<uint8_t>> reedMullerMatrix = generateReedMullerMatrix(m);
    std::vector<uint8_t> encodedMessage;
    encodedMessage.reserve(binaryMessage.size() * n / (m + 1));

    for (size_t i = 0; i < binaryMessage.size(); i += m + 1) {
        std::vector<uint8_t> part(binaryMessage.begin() + i, binaryMessage.begin() + std::min(i + m + 1, binaryMessage.size()));

        // Padding the part to size m + 1
        while (part.size() < static_cast<size_t>(m + 1)) {
            part.push_back(0);
        }

        std::vector<uint8_t> encodedPart(n, 0);
        for (int j = 0; j < n; j++) {
            for (int k = 0; k <= m; k++) {
                encodedPart[j] = static_cast<uint8_t>(encodedPart[j] ^ (reedMullerMatrix[j][k] & part[k]));
            }
        }

        encodedMessage.insert(encodedMessage.end(), encodedPart.begin(), encodedPart.end());
    }

    return encodedMessage;
}

