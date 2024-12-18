#ifndef DECODER_H
#define DECODER_H

#include <vector>
#include <cstdint>

void replaceZeros(std::vector<int8_t> &w);
std::vector<std::vector<int8_t>> hadamardMatrix(int m);
void multiplyByHadamardMatrix(const std::vector<int8_t> &w, const std::vector<std::vector<int8_t>> &H, std::vector<int> &wm);
std::pair<int8_t, std::vector<uint8_t>> findLargestComponent(const std::vector<int> &wm, int m);
std::vector<uint8_t> decodeChunk(const std::vector<uint8_t> &chunk, int m, const std::vector<std::vector<int8_t>> &H);
std::vector<uint8_t> decode(const std::vector<uint8_t> &receivedMessage, int m);
void printHadamardMatrix(const std::vector<std::vector<int8_t>> &H);

#endif // DECODER_H