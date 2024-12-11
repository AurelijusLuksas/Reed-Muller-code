#ifndef STRINGOPERATIONS_H
#define STRINGOPERATIONS_H

#include <string>
#include <vector>

std::vector<bool> stringToBinary(const std::string& text);
std::string binaryToString(const std::vector<bool>& binaryMessage);

#endif // STRINGOPERATIONS_H