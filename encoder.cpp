#include <vector>
#include "encoder.h"
#include <iostream>

// Generate the Reed-Muller generator matrix for RM(1, m)
std::vector<std::vector<bool>> generateReedMullerMatrix(int m) {
    int n = 1 << m; // n = 2^m, the number of columns in the generator matrix
    std::vector<std::vector<bool>> matrix(n, std::vector<bool>(m + 1, 0)); // Initialize the matrix with zeros

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
/**
 * @brief Encodes a binary message using the Reed-Muller code.
 *
 * This function takes a binary message and encodes it using the Reed-Muller code of order m.
 * The Reed-Muller code is a type of error-correcting code that can detect and correct multiple errors.
 *
 * @param binaryMessage The binary message to be encoded, represented as a vector of boolean values.
 * @param m The order of the Reed-Muller code. The length of the encoded message will be 2^m.
 * @return A vector of boolean values representing the encoded message.
 *
 * The function works by dividing the input binary message into parts of size m+1, padding each part if necessary,
 * and then encoding each part using the Reed-Muller matrix. The encoded parts are concatenated to form the final encoded message.
 */
std::vector<bool> encodeMessage(const std::vector<bool>& binaryMessage, int m) {
    int n = 1 << m; // n = 2^m
    std::vector<std::vector<bool>> reedMullerMatrix = generateReedMullerMatrix(m);
    std::vector<bool> encodedMessage;
    encodedMessage.reserve(binaryMessage.size() * n / (m + 1));

    for (size_t i = 0; i < binaryMessage.size(); i += m + 1) {
        std::vector<bool> part(binaryMessage.begin() + i, binaryMessage.begin() + std::min(i + m + 1, binaryMessage.size()));

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

        encodedMessage.insert(encodedMessage.end(), encodedPart.begin(), encodedPart.end());
    }

    return encodedMessage;
}

