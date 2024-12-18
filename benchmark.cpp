#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <cstdint>
#include "encoder.h"  // Assuming encoder.h is defined in the workspace
#include "decoder.h"  // Assuming decoder.h is defined in the workspace
#include "stringOperations.h" // Assuming stringOperations.h is defined in the workspace
#include "channel.h" // Assuming channel.h is defined in the workspace

void runText(int M, const std::string &text, double q, double &encodeTime, double &decodeTime) {
    // Convert text to binary
    std::vector<uint8_t> binaryMessage = stringToBinary(text);

    // Encoding message
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<uint8_t> codedMessage = encodeMessage(binaryMessage, M);
    auto end = std::chrono::high_resolution_clock::now();
    encodeTime = std::chrono::duration<double>(end - start).count();

    // Introduce errors
    std::vector<uint8_t> corruptedMessage = introduceErrors(codedMessage, q);

    // Decoding the message
    start = std::chrono::high_resolution_clock::now();
    std::vector<uint8_t> decodedMessage = decode(corruptedMessage, M);
    end = std::chrono::high_resolution_clock::now();
    decodeTime = std::chrono::duration<double>(end - start).count();
}

void runErrorCorrectionBenchmark(int M, const std::string &text, double q, int &totalErrors, int &correctedErrors) {
    // Convert text to binary
    std::vector<uint8_t> binaryMessage = stringToBinary(text);

    // Encoding message
    std::vector<uint8_t> codedMessage = encodeMessage(binaryMessage, M);

    // Introduce errors
    std::vector<uint8_t> corruptedMessage = introduceErrors(codedMessage, q);

    // Count total errors introduced
    totalErrors = 0;
    for (size_t i = 0; i < codedMessage.size(); ++i) {
        if (codedMessage[i] != corruptedMessage[i]) {
            ++totalErrors;
        }
    }

    // Decoding the message
    std::vector<uint8_t> decodedMessage = decode(corruptedMessage, M);

    // Resize decodedMessage to match the size of codedMessage
    decodedMessage.resize(binaryMessage.size());

    // Count corrected errors
    correctedErrors = 0;
    for (size_t i = 0; i < decodedMessage.size(); ++i) {
        if (codedMessage[i] != decodedMessage[i]) {
            ++correctedErrors;
        }
    }

}

int main() {
    std::string text = "testas";
    const int numRuns = 10; // Number of times to run each benchmark
    std::ofstream file("data/benchmark_results.json");
    file << "[\n";
    int maxM = 25;

    for (int M = 1; M <= maxM; ++M) {
        for (double q = 0.0; q <= 0.5; q += 0.05) {
            double totalEncodeTime = 0.0, totalDecodeTime = 0.0;

            for (int i = 0; i < numRuns; ++i) {
                double encodeTime, decodeTime;
                std::cout << "Running benchmark for M = " << M << ", q = " << q << ", run " << i + 1 << " of " << numRuns << std::endl;
                runText(M, text, q, encodeTime, decodeTime);
                totalEncodeTime += encodeTime;
                totalDecodeTime += decodeTime;
            }

            double avgEncodeTime = totalEncodeTime / numRuns;
            double avgDecodeTime = totalDecodeTime / numRuns;

            file << "  {\n";
            file << "    \"M\": " << M << ",\n";
            file << "    \"q\": " << q << ",\n";
            file << "    \"uzkodavimo_laikas\": " << avgEncodeTime << ",\n";
            file << "    \"dekodavimo_laikas\": " << avgDecodeTime << "\n";
            file << "  }";
            if (!(M == maxM && q >= 0.45)) {
                file << ",";
            }
            file << "\n";

            // Print progress to the console
            std::cout << "M: " << M << ", q: " << q << ", Average Encoding Time: " << avgEncodeTime << ", Average Decoding Time: " << avgDecodeTime << std::endl;
        }
    }

    file << "]";
    file.close();

    // Run error correction benchmark
    std::ofstream errorFile("data/error_correction_results.json");
    errorFile << "[\n";

    for (int M = 1; M <= maxM; ++M) {
        for (double q = 0.05; q <= 0.5; q += 0.05) {
            int totalErrors = 0, correctedErrors = 0;
            runErrorCorrectionBenchmark(M, text, q, totalErrors, correctedErrors);

            errorFile << "  {\n";
            errorFile << "    \"M\": " << M << ",\n";
            errorFile << "    \"q\": " << q << ",\n";
            errorFile << "    \"total_errors\": " << totalErrors << ",\n";
            errorFile << "    \"corrected_errors\": " << totalErrors - correctedErrors << "\n";
            errorFile << "  }";
            if (!(M == maxM && q >= 0.45)) {
                errorFile << ",";
            }
            errorFile << "\n";

            // Print progress to the console
            std::cout << "M: " << M << ", q: " << q << ", Total Errors: " << totalErrors << ", Corrected Errors: " << totalErrors - correctedErrors << std::endl;
        }
    }

    errorFile << "]";
    errorFile.close();

    std::ofstream errorFile2("data/.json");

    return 0;
}