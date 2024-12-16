#ifndef STRINGOPERATIONS_H
#define STRINGOPERATIONS_H

#include <string>
#include <vector>
#include <cstdint>

std::vector<uint8_t> stringToBinary(const std::string& text);
std::string binaryToString(const std::vector<uint8_t>& binaryMessage);

#endif // STRINGOPERATIONS_H