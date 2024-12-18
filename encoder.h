#ifndef ENCODER_H
#define ENCODER_H

#include <vector>
#include <cstdint>

void generateReedMullerMatrix(int m, std::vector<std::vector<uint8_t>>& matrix, size_t n);
std::vector<uint8_t> encodeMessage(const std::vector<uint8_t>& binaryMessage, int m);

#endif // ENCODER_H