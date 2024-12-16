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
    std::cout << "Dvejetaine zinute: ";
    for (size_t i = 0; i < binaryMessage.size(); i++) {
        std::cout << binaryMessage[i];
        if ((i + 1) % 4 == 0) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
    
    std::vector<bool> curruptedWithoutEncoding = introduceErrors(binaryMessage, q);

    std::string TextWithoutEncoding = binaryToString(curruptedWithoutEncoding);

    // Encoding message
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<bool> codedMessage = encodeMessage(binaryMessage, m);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Uzkodavimas uztruko: " << duration.count() << " sekundes" << std::endl << std::endl;

    std::vector<bool> corruptedMessage = introduceErrors(codedMessage, q);

    int totalErrors = 0;
    for (size_t i = 0; i < codedMessage.size(); ++i) {
        if (codedMessage[i] != corruptedMessage[i]) {
            ++totalErrors;
        }
    }

    // Decoding the chunks
    auto decodeStart = std::chrono::high_resolution_clock::now();
    std::vector<bool> decodedMessage = decode(corruptedMessage, m);
    auto decodeEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> decodeDuration = decodeEnd - decodeStart;
    decodedMessage.resize(binaryMessage.size());
    std::cout << "Dekoduota dvejetaine zinute: ";
    for (bool bit : decodedMessage) {
        std::cout << bit;
    }
    
    std::cout << "\nDekodavimas uztruko: " << decodeDuration.count() << " sekundes" << std::endl;

    std::string decodedText = binaryToString(decodedMessage);
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Tekstas be uzkodavimo: " << TextWithoutEncoding << std::endl;
    std::cout << std::endl;
    std::cout << "Dekoduotas tesktas: " << decodedText << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    binaryMessage.clear();
    codedMessage.clear();
    corruptedMessage.clear();
    decodedMessage.clear();

}

void runVector(int m, std::vector<bool> vector, float q) {
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Pradinis vektorius: ";
    for (size_t i = 0; i < vector.size(); i++) {
        std::cout << vector[i];
        if ((i + 1) % 4 == 0) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<bool> codedMessage = encodeMessage(vector, m);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Uzkodavimas uztruko: " << duration.count() << " sekundes" << std::endl;
    std::cout << "Uzkoduoto vektorio ilgis: " << codedMessage.size() << std::endl;

    std::vector<bool> corruptedMessage = introduceErrors(codedMessage, q);
    int totalErrorsBefore = 0;

    for (size_t i = 0; i < codedMessage.size(); ++i) {
        if (codedMessage[i] != corruptedMessage[i]) {
            ++totalErrorsBefore;
        }
    }

    std::cout <<"Uzkoduotas vektorius: ";
    for (size_t i = 0; i < codedMessage.size(); i++) {
        std::cout << codedMessage[i];
        if ((i + 1) % 4 == 0) {
            std::cout << " ";
        }
    }

    std::cout << std::endl;

    std::cout << "Is kanalo gautas vektorius: ";
    for (size_t i = 0; i < corruptedMessage.size(); i++) {
        std::cout << corruptedMessage[i];
        if ((i + 1) % 4 == 0) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
    std::cout << "Klaidingu bit'u pozicijos:  ";
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
    std::cout << "Ar norite pakeisti klaidinga vektoriu? (y/n): ";
    char choice;
    std::cin >> choice;
    if (choice == 'y') {
        std::cout << "Iveskite klaidinga vektoriu: ";
        std::string input;
        std::cin >> input;
        corruptedMessage.resize(input.size());
        for (size_t i = 0; i < input.size(); i++) {
            corruptedMessage[i] = input[i] - '0';
        }
    }


    auto decodeStart = std::chrono::high_resolution_clock::now();
    std::vector<bool> decodedMessage = decode(corruptedMessage, m);
    auto decodeEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> decodeDuration = decodeEnd - decodeStart;
    decodedMessage.resize(vector.size());
    std::cout << "Dekoduotas vektorius: ";
    for (size_t i = 0; i < decodedMessage.size(); i++) {
        std::cout << decodedMessage[i];
        if ((i + 1) % 4 == 0) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
    std::cout << "Dekodavimas uztruko: " << decodeDuration.count() << " sekundes" << std::endl;
    std::cout << std::endl;

    std::cout << "--------------------------------------------------" << std::endl;
}

int main() {
    std::string text;
    std::string filename;
    std::vector<bool> vector;
    int m;
    float q;
    std::string input;              
    std::string input2;
    while (true) {
        std::cout << "[1] Uzkoduoti vektoriu\n";
        std::cout << "[2] Uzkoduoti teksta\n";
        std::cout << "[3] Uzkoduoti nuotrauka\n";
        std::cout << "[4] Isjungti\n";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
            std::cout << "Iveskite m: ";
            while (true) {
                std::cin >> m;
                if (std::cin.fail()) {
                    std::cout << "Neteisinga ivestis. Iveskite sveikaji skaiciu: ";
                    std::cin.clear(); 
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                } else {
                    break;
                }
            }

            vector.resize(m + 1);
            std::cout << "Iveskite vektoriu ilgio " << m + 1 << ": ";
            std::cin >> input;

            for (int i = 0; i < m + 1; ++i) {
                vector[i] = input[i] - '0';
            }


            std::cout << "Iveskite klaidu procenta: ";
            std::cin >> input2;
            std::replace(input2.begin(), input2.end(), ',', '.');

            q = std::stof(input2);

            runVector(m, vector, q);
                        
            break;
            case 2:
            // Encode text
            std::cout << "Iveskite m: ";
            while (true) {
                std::cin >> m;
                if (std::cin.fail()) {
                    std::cout << "Neteisinga ivestis. Iveskite sveikaji skaiciu: ";
                    std::cin.clear(); 
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                } else {
                    break;
                }
            }
            
            std::cout << "Iveskite zinute: ";
            std::cin >> text;

            std::cout << "Iveskite klaidu procenta: ";
            std::cin >> input2;
            std::replace(input2.begin(), input2.end(), ',', '.');

            q = std::stof(input2);

            runText(m, text, q);

            break;
            case 3:
            // Encode picture
            std::cout << "Iveskite m: ";
            while (true) {
                std::cin >> m;
                if (std::cin.fail()) {
                    std::cout << "Neteisinga ivestis. Iveskite sveikaji skaiciu: ";
                    std::cin.clear(); 
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                } else {
                    break;
                }
            }

            std::cout << "Iveskite paveikslelio pavadinima: ";
            std::cin >> filename;

            std::cout << "Iveskite klaidu procenta: ";
            std::cin >> input2;
            std::replace(input2.begin(), input2.end(), ',', '.');

            q = std::stof(input2);

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