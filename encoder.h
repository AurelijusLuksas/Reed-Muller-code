#ifndef ENCODER_H
#define ENCODER_H

#include <vector>

std::vector<std::vector<bool>> generateReedMullerMatrix(int m);
std::vector<bool> encodeMessage(const std::vector<bool>& binaryMessage, int m);

#endif // ENCODER_H