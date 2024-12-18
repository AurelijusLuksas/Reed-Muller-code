#ifndef DECODERRECURSIVE_H
#define DECODERRECURSIVE_H

#include <vector>
#include <cstdint>

void fastHadamardTransformRecursive(std::vector<int>& vec, size_t start, size_t end);
std::vector<int> fastHadamardTransform(const std::vector<uint8_t>& message, int m);
std::vector<uint8_t> reverseVector(const std::vector<uint8_t>& vector);
std::pair<int, int> findLargestComponentPosition(const std::vector<int>& vector);
std::vector<uint8_t> decodeChunks(std::vector<uint8_t> message, int m);
std::vector<std::vector<uint8_t>> splitMessageForDecoding(const std::vector<uint8_t>& message, size_t chunkSize);
std::vector<uint8_t> decode(std::vector<uint8_t> message, int m);

#endif // DECODERRECURSIVE_H