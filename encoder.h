#ifndef ENCODER_H
#define ENCODER_H

#include <vector>
#include <cstdint>

void encodeRecursive(const std::vector<uint8_t>& binaryMessage, size_t m, std::vector<uint8_t>& encodedMessage, size_t offset, size_t stride);
std::vector<uint8_t> encodeMessage(const std::vector<uint8_t>& binaryMessage, size_t m);

#endif // ENCODER_H