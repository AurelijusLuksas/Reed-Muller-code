#include <vector>
#include "encoder.h"
#include <iostream>
#include <omp.h>


// Generate the Reed-Muller generator matrix for RM(1, m)
std::vector<std::vector<bool>> generateReedMullerMatrix(int m) {
    int n = 1 << m; // n = 2^m
    std::vector<std::vector<bool>> matrix(n, std::vector<bool>(m + 1, 0));

    for (int i = 0; i < n; i++) {
        matrix[i][0] = 1; // Constant term
        for (int j = 1; j <= m; j++) {
            matrix[i][j] = (i & (1 << (j - 1))) ? 1 : 0;
        }
    }

    return matrix;
}

// Encode the binary message using Reed-Muller RM(1, m)
std::vector<bool> encodeMessage(const std::vector<bool>& binaryMessage, int m) {
    int n = 1 << m; // n = 2^m
    std::vector<std::vector<bool>> reedMullerMatrix = generateReedMullerMatrix(m);
    std::vector<bool> encodedMessage;
    encodedMessage.reserve(binaryMessage.size() * n / (m + 1));

    // #pragma omp parallel for
    for (size_t i = 0; i < binaryMessage.size(); i += m + 1) {
        std::vector<bool> part(binaryMessage.begin() + i, 
                              binaryMessage.begin() + std::min(i + m + 1, binaryMessage.size()));

        // Padding the part to size m + 1
        while (part.size() < static_cast<size_t>(m + 1)) {
            part.push_back(0);
        }

        std::vector<bool> encodedPart(n, 0);
        for (int j = 0; j < n; j++) {
            for (int k = 0; k <= m; k++) {
                encodedPart[j] = static_cast<bool>(static_cast<int>(encodedPart[j]) ^ (static_cast<int>(reedMullerMatrix[j][k]) & static_cast<int>(part[k])));
            }
        }

        // #pragma omp critical
        encodedMessage.insert(encodedMessage.end(), encodedPart.begin(), encodedPart.end());
    }

    return encodedMessage;
}

