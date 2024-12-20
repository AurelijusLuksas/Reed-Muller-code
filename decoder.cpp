#include <vector>
#include <cmath>
#include <iostream>
#include "decoder.h"
#include <cstdint>
#include <algorithm>
#include <chrono>
#include <unordered_map>

// Helper functions
std::vector<std::vector<uint8_t>> splitMessageForDecoding(const std::vector<uint8_t>& message, int m) {
    size_t numChunks = (message.size() + m - 1) / m;
    std::vector<std::vector<uint8_t>> chunks(numChunks, std::vector<uint8_t>(m, 0));

    for (size_t i = 0; i < message.size(); ++i) {
        chunks[i / m][i % m] = message[i];
    }

    return chunks;
}


std::vector<int> convertToPm1(const std::vector<uint8_t>& message) {
    std::vector<int> pm1(message.size());
    for (size_t i = 0; i < message.size(); ++i) {
        pm1[i] = message[i] ? 1 : -1;
    }
    return pm1;
}



std::vector<std::vector<int>> generateUnitaryMatrix(int n) {
    if (n < 1) {
        throw std::invalid_argument("n must be greater than 0");
    }
    std::vector<std::vector<int>> matrix(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        matrix[i][i] = 1;
    }
    return matrix;
}

std::vector<std::vector<int>> generateKroneckerProduct(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    size_t aRows = A.size(), aCols = A[0].size();
    size_t bRows = B.size(), bCols = B[0].size();

    std::vector<std::vector<int>> result(aRows * bRows, std::vector<int>(aCols * bCols));

    for (size_t i = 0; i < aRows; ++i) {
        for (size_t j = 0; j < aCols; ++j) {
            int value = A[i][j];
            for (size_t k = 0; k < bRows; ++k) {
                for (size_t l = 0; l < bCols; ++l) {
                    result[i * bRows + k][j * bCols + l] = value * B[k][l];
                }
            }
        }
    }
    return result;
}

std::unordered_map<int, std::vector<std::vector<int>>> HiMCache;

std::vector<std::vector<int>> generateHiM(int i, int m) {
    int key = (i << 16) | m;
    if (HiMCache.find(key) != HiMCache.end()) {
        return HiMCache[key];
    }

    int size1 = 1 << (m - i);
    int size2 = 1 << (i - 1);

    std::vector<std::vector<int>> I1(size1, std::vector<int>(size1, 0));
    for (int j = 0; j < size1; ++j) I1[j][j] = 1;

    std::vector<std::vector<int>> H = {{1, 1}, {1, -1}};
    auto HiM = generateKroneckerProduct(I1, H);

    std::vector<std::vector<int>> I2(size2, std::vector<int>(size2, 0));
    for (int j = 0; j < size2; ++j) I2[j][j] = 1;

    HiMCache[key] = generateKroneckerProduct(HiM, I2);
    return HiMCache[key];
}

std::vector<uint8_t> intToUnpackedBitList(int n, int m) {
    std::vector<uint8_t> bitArray(m, 0);
    for (int i = 0; i < m; ++i) {
        bitArray[i] = (n >> i) & 1;
    }
    return bitArray;
}


std::vector<int> dotProduct(const std::vector<int>& v1, const std::vector<int>& v2) {
    if (v1.size() != v2.size()) {
        throw std::invalid_argument("Vectors must have the same length");
    }
    std::vector<int> result(v1.size());
    for (size_t i = 0; i < v1.size(); ++i) {
        result[i] = v1[i] * v2[i];
    }
    return result;
}

std::vector<int> vectorByMatrix(const std::vector<int>& vector, const std::vector<std::vector<int>>& matrix) {
    std::vector<int> result(matrix.size());
    for (size_t i = 0; i < matrix.size(); ++i) {
        int sum = 0;
        for (size_t j = 0; j < vector.size(); ++j) {
            sum += vector[j] * matrix[i][j];
        }
        result[i] = sum;
    }
    return result;
}

void fastHadamardTransform(const std::vector<uint8_t>& message, int m, std::vector<int>& transformedMessage) {
    transformedMessage = convertToPm1(message);

    for (int i = 1; i <= m; ++i) {
        transformedMessage = vectorByMatrix(transformedMessage, generateHiM(i, m));
    }
}


std::vector<uint8_t> reverseVector(const std::vector<uint8_t>& vector) {
    std::vector<uint8_t> reversed(vector.size());
    std::reverse_copy(vector.begin(), vector.end(), reversed.begin());
    return reversed;
}


// New decode function
std::vector<uint8_t> decodeChunks(std::vector<uint8_t> message, int m) {
    std::vector<int> transformedMessage;
    fastHadamardTransform(message, m, transformedMessage);
    
    // Find the largest component position and sign
    int largestValue = 0;
    int position = 0;
    for (size_t i = 0; i < transformedMessage.size(); ++i) {
        int absValue = std::abs(transformedMessage[i]);
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

std::vector<uint8_t> decode(std::vector<uint8_t> message, int m) {
    std::vector<uint8_t> decoded;
    std::vector<std::vector<uint8_t>> chunks = splitMessageForDecoding(message, 1 << m);
    for (const auto& chunk : chunks) {
        std::vector<uint8_t> decodedChunk = decodeChunks(chunk, m);
        decoded.insert(decoded.end(), decodedChunk.begin(), decodedChunk.end());
    }

    return decoded;
}