#ifndef ENCODER_H
#define ENCODER_H

#include <vector>
#include <cstdint>

std::vector<std::vector<uint8_t>> generateReedMullerMatrix(int m);
std::vector<uint8_t> encodeMessage(const std::vector<uint8_t>& binaryMessage, int m);

#endif // ENCODER_H