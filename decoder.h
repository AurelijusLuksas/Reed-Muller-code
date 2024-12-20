#ifndef DECODER_H
#define DECODER_H

#include <vector>
#include <cstdint>
#include <unordered_map>

std::vector<std::vector<uint8_t>> splitMessageForDecoding(const std::vector<uint8_t>& message, int m);
std::vector<int> convertToPm1(const std::vector<uint8_t>& message);
std::vector<std::vector<int>> generateUnitaryMatrix(int n);
std::vector<std::vector<int>> generateKroneckerProduct(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B);
std::vector<std::vector<int>> generateHiM(int i, int m);
std::vector<uint8_t> intToUnpackedBitList(int n, int m);
std::vector<int> dotProduct(const std::vector<int>& v1, const std::vector<int>& v2);
std::vector<int> vectorByMatrix(const std::vector<int>& vector, const std::vector<std::vector<int>>& matrix);
void fastHadamardTransform(const std::vector<uint8_t>& message, int m, std::vector<int>& transformedMessage);
std::vector<uint8_t> reverseVector(const std::vector<uint8_t>& vector);
std::vector<uint8_t> decodeChunks(std::vector<uint8_t> message, int m);
std::vector<uint8_t> decode(std::vector<uint8_t> message, int m);


#endif // DECODER_H