#ifndef DECODER_H
#define DECODER_H

#include <vector>
#include <cstdint>

void replaceZeros(std::vector<int8_t> &w);
void hadamardTransform(std::vector<int8_t>& vec);
std::vector<int> applyHadamardTransform(const std::vector<int8_t>& w);
std::pair<int8_t, std::vector<uint8_t>> findLargestComponent(const std::vector<int> &wm, int m);
std::vector<uint8_t> decodeChunk(const std::vector<uint8_t> &chunk, int m);
std::vector<uint8_t> decode(const std::vector<uint8_t> &receivedMessage, int m);


#endif // DECODER_H