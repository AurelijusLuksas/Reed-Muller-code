#ifndef DECODER_H
#define DECODER_H

#include <vector>

void replaceZeros(std::vector<int> &w);
std::vector<std::vector<int>> hadamardMatrix(int m);
std::vector<int> multiplyByHadamardMatrix(const std::vector<int> &w, int m);
std::pair<int, std::vector<int>>  findLargestComponent(const std::vector<int> &wm, int m);
std::vector<bool> decodeChunk(const std::vector<bool> &chunk, int m);
std::vector<bool> decode(const std::vector<bool> &receivedMessage, int m);


#endif // DECODER_H