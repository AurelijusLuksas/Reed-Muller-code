#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <chrono>
#include "encoder.h"
#include "decoder.h"
#include "stringOperations.h"
#include "channel.h"
#include "picture.h"
#include <cstdint>

void runText(int m, std::string text, float q) {

    // Convert text to binary
    std::vector<uint8_t> binaryMessage = stringToBinary(text);
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Binary message: ";
    for (size_t i = 0; i < binaryMessage.size(); i++) {
        std::cout << static_cast<int>(binaryMessage[i]); // Cast to int for correct display
        if ((i + 1) % 4 == 0) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
    
    std::vector<uint8_t> curruptedWithoutEncoding = introduceErrors(binaryMessage, q);

    std::string TextWithoutEncoding = binaryToString(curruptedWithoutEncoding);

    // Encoding message
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<uint8_t> codedMessage = encodeMessage(binaryMessage, m);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Encoding took: " << duration.count() << " seconds" << std::endl << std::endl;

    std::vector<uint8_t> corruptedMessage = introduceErrors(codedMessage, q);

    int totalErrors = 0;
    for (size_t i = 0; i < codedMessage.size(); ++i) {
        if (codedMessage[i] != corruptedMessage[i]) {
            ++totalErrors;
        }
    }

    // Decoding the chunks
    auto decodeStart = std::chrono::high_resolution_clock::now();
    std::vector<uint8_t> decodedMessage = decode(corruptedMessage, m);
    auto decodeEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> decodeDuration = decodeEnd - decodeStart;
    decodedMessage.resize(binaryMessage.size());
    std::cout << "Decoded binary message: ";
    for (size_t i = 0; i < decodedMessage.size(); i++) {
        std::cout << static_cast<int>(decodedMessage[i]); // Cast to int for correct display
        if ((i + 1) % 4 == 0) {
            std::cout << " ";
        }
    }

    std::cout << "\nDecoding took: " << decodeDuration.count() << " seconds." << std::endl;

    std::string decodedText = binaryToString(decodedMessage);
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Text without encoding: " << TextWithoutEncoding << std::endl;
    std::cout << std::endl;
    std::cout << "Decoded text: " << decodedText << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    binaryMessage.clear();
    codedMessage.clear();
    corruptedMessage.clear();
    decodedMessage.clear();

}

void runVector(int m, std::vector<uint8_t> vector, float q) {
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Starting vector: ";
    for (size_t i = 0; i < vector.size(); i++) {
        std::cout << static_cast<int>(vector[i]);
        if ((i + 1) % 4 == 0) {
            std::cout << " ";
        }
    }

    std::cout << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<uint8_t> codedMessage = encodeMessage(vector, m);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Encoding took: " << duration.count() << " seconds" << std::endl;
    std::cout << "Encoded vector length: " << codedMessage.size() << std::endl;

    std::vector<uint8_t> corruptedMessage = introduceErrors(codedMessage, q);
    int totalErrorsBefore = 0;

    for (size_t i = 0; i < codedMessage.size(); ++i) {
        if (codedMessage[i] != corruptedMessage[i]) {
            ++totalErrorsBefore;
        }
    }

    std::cout << std::endl;

    std::cout << "Vector received from channel: ";
    for (size_t i = 0; i < corruptedMessage.size(); i++) {
        std::cout << static_cast<int>(corruptedMessage[i]);
        if ((i + 1) % 4 == 0) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
    std::cout << "Positions of erroneous bits:  ";
    for (size_t i = 0; i < corruptedMessage.size(); i++) {
        if (codedMessage[i] != corruptedMessage[i]) {
            std::cout << "^";
        } else {
            std::cout << "-";
        }
        if ((i + 1) % 4 == 0) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
    std::cout << "Do you want to change incorrect vector? (y/n): ";
    char choice;
    std::cin >> choice;
    if (choice == 'y') {
        std::cout << "Input the incorrect vector: ";
        std::string input;
        std::cin >> input;
        corruptedMessage.resize(input.size());
        for (size_t i = 0; i < input.size(); i++) {
            corruptedMessage[i] = input[i] - '0';
        }
    }


    auto decodeStart = std::chrono::high_resolution_clock::now();
    std::vector<uint8_t> decodedMessage = decode(corruptedMessage, m);
    auto decodeEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> decodeDuration = decodeEnd - decodeStart;
    decodedMessage.resize(vector.size());
    std::cout << "Decoded vector: ";
    for (size_t i = 0; i < decodedMessage.size(); i++) {
        std::cout << static_cast<int>(decodedMessage[i]);
        if ((i + 1) % 4 == 0) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
    std::cout << "Decoding took: " << decodeDuration.count() << " seconds" << std::endl;
    std::cout << std::endl;

    std::cout << "--------------------------------------------------" << std::endl;
}

int main() {
    std::string text;
    std::string filename;
    std::vector<uint8_t> vector;
    int m;
    float q;
    std::string input;              
    std::string input2;
    while (true) {
        std::cout << "[1] Encode vector\n";
        std::cout << "[2] Encode text\n";
        std::cout << "[3] Encode photo\n";
        std::cout << "[4] Quit\n";

        int choice;
        std::cin >> choice;
        std::string line;
        switch (choice) {
            case 1:
            std::cout << "Input m: ";
            while (true) {
                std::cin >> m;
                if (std::cin.fail()) {
                    std::cout << "Incorrect input. Please enter an integer: ";
                    std::cin.clear(); 
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                } else {
                    break;
                }
            }

            vector.resize(m + 1);
            std::cout << "Input vector length of " << m + 1 << ": ";
            std::cin >> input;

            for (int i = 0; i < m + 1; ++i) {
                vector[i] = input[i] - '0';
            }


            std::cout << "Input error quantity (0 - 1): ";
            std::cin >> input2;
            std::replace(input2.begin(), input2.end(), ',', '.');

            q = std::stof(input2);

            runVector(m, vector, q);
                        
            break;
            case 2:
            // Encode text
            std::cout << "Input m: ";
            while (true) {
                std::cin >> m;
                if (std::cin.fail()) {
                    std::cout << "Incorrect input. Please enter an integer: ";
                    std::cin.clear(); 
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                } else {
                    break;
                }
            }
            
            std::cout << "Input text (write END to stop input): ";
            std::cin.ignore(); // Clear the input buffer
            text.clear();
            while (true) {
                std::getline(std::cin, line);
                if (line == "END") {
                    break;
                }
                text += line + "\n";
            }

            std::cout << "Input error quantity (0 - 1): ";
            std::cin >> input2;
            std::replace(input2.begin(), input2.end(), ',', '.');

            q = std::stof(input2);

            runText(m, text, q);

            break;
            case 3:
            // Encode picture
            std::cout << "Input m: ";
            while (true) {
                std::cin >> m;
                if (std::cin.fail()) {
                    std::cout << "Incorrect input. Please enter an integer: ";
                    std::cin.clear(); 
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                } else {
                    break;
                }
            }

            std::cout << "Input photo name (.bmp): ";
            std::cin >> filename;

            std::cout << "Input error quantity (0 - 1): ";
            std::cin >> input2;
            std::replace(input2.begin(), input2.end(), ',', '.');

            q = std::stof(input2);

            runPicture(m, filename, q);

            break;
            case 4:
            // Exit
            std::cout << "Quitting...." << std::endl;
            return 0;
            default:
            std::cout << "Incorrect choice. Please try again." << std::endl;
            break;
        }
    }

    return 0;
}