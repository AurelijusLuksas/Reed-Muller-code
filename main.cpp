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

void runText(int m, std::string text, float q) {

    // Convert text to binary
    std::vector<bool> binaryMessage = stringToBinary(text);
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Binary message: ";
    for (size_t i = 0; i < binaryMessage.size(); i++) {
        std::cout << binaryMessage[i];
        if ((i + 1) % 4 == 0) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
    
    // Encoding message
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<bool> codedMessage = encodeMessage(binaryMessage, m);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Encoded length: " << codedMessage.size() << std::endl;

    std::cout << "Encoding took: " << duration.count() << " seconds" << std::endl << std::endl;

    std::vector<bool> corruptedMessage = introduceErrors(codedMessage, q);

    int totalErrors = 0;
    for (size_t i = 0; i < codedMessage.size(); ++i) {
        if (codedMessage[i] != corruptedMessage[i]) {
            ++totalErrors;
        }
    }

    std::cout << "Total amount of errors: " << totalErrors << std::endl << std::endl;

    // std::cout << "Encoded message: ";
    // for (bool bit : codedMessage) {
    //     std::cout << bit;
    // }
    // std::cout << std::endl;

    // Decoding the chunks
    auto decodeStart = std::chrono::high_resolution_clock::now();
    std::vector<bool> decodedMessage = decodeChunks(corruptedMessage, 1, m);
    auto decodeEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> decodeDuration = decodeEnd - decodeStart;
    decodedMessage.resize(binaryMessage.size());
    std::cout << "Decoded binary message: ";
    for (bool bit : decodedMessage) {
        std::cout << bit;
    }
    
    std::cout << "\nDecoding took: " << decodeDuration.count() << " seconds" << std::endl;

    std::string decodedText = binaryToString(decodedMessage);
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Decoded text: " << decodedText << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    binaryMessage.clear();
    codedMessage.clear();
    corruptedMessage.clear();
    decodedMessage.clear();

}

int main() {
    std::string text;
    std::string filename;
    int m;
    float q;

    while (true) {
        std::cout << "[1] Encode vector\n";
        std::cout << "[2] Encode text\n";
        std::cout << "[3] Encode picture\n";
        std::cout << "[4] Exit\n";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
            // Encode vector
            std::cout << "Vector encoding not implemented yet." << std::endl;
            break;
            case 2:
            // Encode text
            std::cout << "Iveskite m: "; 
            std::cin >> m;
            
            std::cout << "Iveskite zinute: ";
            std::cin >> text;

            std::cout << "Iveskite klaidu procenta: ";
            std::cin >> q;

            runText(m, text, q);

            break;
            case 3:
            // Encode picture
            std::cout << "Iveskite m: ";
            std::cin >> m;

            std::cout << "Iveskite paveikslelio pavadinima: ";
            std::cin >> filename;

            std::cout << "Iveskite klaidu procenta: ";
            std::cin >> q;

            runPicture(m, filename, q);

            break;
            case 4:
            // Exit
            std::cout << "Programa isjungiama." << std::endl;
            return 0;
            default:
            std::cout << "Neteisingas pasirinkimas. Bandykite is naujo." << std::endl;
            break;
        }
    }

    return 0;
}