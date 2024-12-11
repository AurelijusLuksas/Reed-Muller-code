#ifndef DECODER_H
#define DECODER_H

#include <vector>

void fastHadamardTransformRecursive(std::vector<int>& vec, size_t start, size_t end);
std::vector<int> fastHadamardTransform(const std::vector<bool>& message, int m);
std::vector<bool> reverseVector(const std::vector<bool>& vector);
std::vector<bool> intToUnpackedBitList(int number);
std::pair<int, int> findLargestComponentPosition(const std::vector<int>& transformedMessage);
std::vector<bool> decode(std::vector<bool> message, int r, int m);
std::vector<std::vector<bool>> splitMessageForDecoding(const std::vector<bool>& message, size_t chunkSize);
std::vector<bool> decodeChunks(std::vector<bool> message, int r, int m);

#endif // DECODER_H