#include <vector>
#include <cmath>
#include <iostream>
#include "decoder.h"

// Recursive Fast Hadamard Transform
void fastHadamardTransformRecursive(std::vector<int>& vec, size_t start, size_t end) {
    if (end - start == 1) return; // Base case: single element

    size_t mid = start + (end - start) / 2;
    fastHadamardTransformRecursive(vec, start, mid); // first half
    fastHadamardTransformRecursive(vec, mid, end);   // second half

    for (size_t i = start; i < mid; ++i) {
        int a = vec[i];
        int b = vec[i + (mid - start)];
        vec[i] = a + b;               // (sum)
        vec[i + (mid - start)] = a - b; // (difference)
    }
}

// Fast Hadamard Transform (driver)
std::vector<int> fastHadamardTransform(const std::vector<bool>& message, int m) {
    size_t N = 1 << m; // (2^m)
    std::vector<int> vector(message.size());
    for (size_t i = 0; i < message.size(); ++i) {
        vector[i] = message[i] ? 1 : -1;
    }

    fastHadamardTransformRecursive(vector, 0, N);

    return vector;
}

std::vector<bool> reverseVector(const std::vector<bool>& vector) {
    std::vector<bool> reversed;
    for (int i = vector.size() - 1; i >= 0; --i) {
        reversed.push_back(vector[i]);
    }
    return reversed;
}

std::vector<bool> intToUnpackedBitList(int n) {
    if (n < 0) {
        throw std::invalid_argument("n must be greater than or equal to 0");
    }
    int length = 0;
    int temp = n;
    while (temp > 0) {
        length++;
        temp >>= 1;
    }
    std::vector<bool> bitArray(length, false);
    for (int i = length - 1; i >= 0; --i) {
        bitArray[i] = n % 2;
        n /= 2;
    }
    return bitArray;
}

std::pair<int, int> findLargestComponentPosition(const std::vector<int>& vector) {
    int max_value = std::abs(vector[0]);
    int position = 0;
    int sign = (vector[0] > 0) ? 1 : -1;
    for (size_t i = 1; i < vector.size(); ++i) {
        if (std::abs(vector[i]) > max_value) {
            max_value = std::abs(vector[i]);
            sign = (vector[i] > 0) ? 1 : -1;
            position = i;
        }
    }
    return {position, sign};
}

std::vector<bool> decode(std::vector<bool> message, int r, int m) {
    std::vector<int> transformedMessage;
    transformedMessage = fastHadamardTransform(message, m);
    std::pair<int, int> result = findLargestComponentPosition(transformedMessage);
    int position = result.first;
    int sign = result.second;

    std::vector<bool> positionInBits = intToUnpackedBitList(position);
    positionInBits = reverseVector(positionInBits);
    while (positionInBits.size() < static_cast<size_t>(m + 1)) {
        positionInBits.push_back(0);
    }
    if (sign == 1) {
        positionInBits.insert(positionInBits.begin(), 1);
        positionInBits.pop_back();
    } else {
        positionInBits.insert(positionInBits.begin(), 0);
        positionInBits.pop_back();
    }

    return positionInBits;
}

std::vector<std::vector<bool>> splitMessageForDecoding(const std::vector<bool>& message, size_t chunkSize) {
    std::vector<std::vector<bool>> chunks;
    size_t appendedBits = 0; 
    for (size_t i = 0; i < message.size(); i += chunkSize) {
        std::vector<bool> chunk(message.begin() + i, message.begin() + std::min(message.size(), i + chunkSize));
        while (chunk.size() < chunkSize) { 
            chunk.push_back(0);
            appendedBits++;
        }
        chunks.push_back(chunk);
    }
    return chunks;
}

std::vector<bool> decodeChunks(std::vector<bool> message, int r, int m) {
    std::vector<bool> decoded;
    std::vector<std::vector<bool>> chunks = splitMessageForDecoding(message, 1 << m);
    for (const auto& chunk : chunks) {
        std::vector<bool> decodedChunk = decode(chunk, r, m);
        decoded.insert(decoded.end(), decodedChunk.begin(), decodedChunk.end());
    }
    return decoded;
}