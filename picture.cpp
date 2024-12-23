#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "encoder.h"
#include "decoder.h"
#include "stringOperations.h"
#include "channel.h"
#include "picture.h"
#include <cstdint>

void runPicture(int m, std::string filename, float q) {
    // Read BMP file
    std::cout << "--------------------------------------------------" << std::endl;
    int width, height, channels;
    std::string inputFilePath = "photos/" + filename;
    unsigned char* pixelData = stbi_load(inputFilePath.c_str(), &width, &height, &channels, 0);
    if (!pixelData) {
        std::cerr << "Error: Couldn't open the file. " << filename << std::endl;
        return;
    }

    // Print out image width and height
    std::cout << "Image width: " << width << std::endl;
    std::cout << "Image height: " << height << std::endl;


    // Convert pixel data to binary
    std::vector<uint8_t> binaryData;
    for (int i = 0; i < width * height * channels; ++i) {
        for (int j = 7; j >= 0; --j) {
            binaryData.push_back((pixelData[i] >> j) & 1);
        }
    }

    std::vector<uint8_t> corruptedWithoutEncoding = introduceErrors(binaryData, q);

    // Convert binary data to pixel data
    std::vector<uint8_t> corruptedPixelData;
    for (size_t i = 0; i < corruptedWithoutEncoding.size(); i += 8) {
        uint8_t byte = 0;
        for (int j = 0; j < 8; ++j) {
            if (i + j < corruptedWithoutEncoding.size()) {
                byte |= (corruptedWithoutEncoding[i + j] << (7 - j));
            }
        }
        corruptedPixelData.push_back(byte);
    }

    // Create the output filename by appending "_corrupted" before the file extension
    std::string corruptedFilename = filename;
    size_t dotPos = corruptedFilename.find_last_of(".");
    if (dotPos != std::string::npos) {
        corruptedFilename.insert(dotPos, "_corrupted_q" + std::to_string(static_cast<int>(q * 100) / 100.0).substr(0, 4));
    } else {
        corruptedFilename += "_corrupted_q" + std::to_string(static_cast<int>(q * 100) / 100.0).substr(0, 4);
    }
    std::string corruptedFilePath = "photos/" + corruptedFilename;

    // Write corrupted pixel data back to BMP file
    if (!stbi_write_bmp(corruptedFilePath.c_str(), width, height, channels, corruptedPixelData.data())) {
        std::cerr << "Error: Couldn't create output file." << std::endl;
        stbi_image_free(pixelData);
        return;
    }

    // Open the corrupted image
    std::string openCommand = "start " + corruptedFilePath;
    system(openCommand.c_str());

    std::cout << "Through channel sent picture save as " << corruptedFilename << std::endl;
    


    // Measure the time taken to encode the binary data
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<uint8_t> encodedData = encodeMessage(binaryData, m);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Encoding took " << duration.count() << " seconds." << std::endl;

    // Introduce errors
    std::vector<uint8_t> corruptedData = introduceErrors(encodedData, q);

    int totalErrorsBefore = 0;
    for (size_t i = 0; i < encodedData.size(); ++i) {
        if (encodedData[i] != corruptedData[i]) {
            ++totalErrorsBefore;
        }
    }

    // Decode the data
    auto decodeStart = std::chrono::high_resolution_clock::now();
    std::vector<uint8_t> decodedData = decode(corruptedData, m);
    auto decodeEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> decodeDuration = decodeEnd - decodeStart;
    std::cout << "Decoding took " << decodeDuration.count() << " seconds." << std::endl;

    int totalErrorsAfter = 0;
    for (size_t i = 0; i < binaryData.size(); ++i) {
        if (binaryData[i] != decodedData[i]) {
            ++totalErrorsAfter;
        }
    }


    // Convert binary data back to pixel data
    std::vector<uint8_t> correctedPixelData;
    for (size_t i = 0; i < decodedData.size(); i += 8) {
        uint8_t byte = 0;
        for (int j = 0; j < 8; ++j) {
            if (i + j < decodedData.size()) {
                byte |= (decodedData[i + j] << (7 - j));
            }
        }
        correctedPixelData.push_back(byte);
    }

    // Create the output filename by appending "_corrected" before the file extension
    std::string outputFilename = filename;
    dotPos = outputFilename.find_last_of(".");
    if (dotPos != std::string::npos) {
        outputFilename.insert(dotPos, "_corrected_m" + std::to_string(m) + "_q" + std::to_string(static_cast<int>(q * 100) / 100.0).substr(0, 4));
    } else {
        outputFilename += "_corrected_m" + std::to_string(m) + "_q" + std::to_string(static_cast<int>(q * 100) / 100.0).substr(0, 4);
    }
    std::string outputFilePath = "photos/" + outputFilename;

    // Write corrected pixel data back to BMP file
    if (!stbi_write_bmp(outputFilePath.c_str(), width, height, channels, correctedPixelData.data())) {
        std::cerr << "Error: Couldn't create output file." << std::endl;
        stbi_image_free(pixelData);
        return;
    }

    // Open the corrected image
    openCommand = "start " + outputFilePath;
    system(openCommand.c_str());

    std::cout << "Total errors: " << totalErrorsBefore << std::endl;
    std::cout << "Corrected errors: " << totalErrorsBefore - totalErrorsAfter << std::endl;

    stbi_image_free(pixelData);
    std::cout << "Encoded and decoded picture saved as " << outputFilename << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
}