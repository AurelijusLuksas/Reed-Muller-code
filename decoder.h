#ifndef DECODERRECURSIVE_H
#define DECODERRECURSIVE_H

#include <vector>
#include <cstdint>

void fastHadamardTransforms(std::vector<int>& vec);
std::vector<uint8_t> decodeChunks(const std::vector<uint8_t>& message, int m);
std::vector<std::vector<uint8_t>> splitMessageForDecoding(const std::vector<uint8_t>& message, size_t chunkSize);
std::vector<uint8_t> decode(const std::vector<uint8_t>& message, int m);

#endif // DECODERRECURSIVE_H