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

void runPicture(int m, std::string filename, float q) {
    // Read BMP file
    std::cout << "--------------------------------------------------" << std::endl;
    int width, height, channels;
    std::string inputFilePath = "photos/" + filename;
    unsigned char* pixelData = stbi_load(inputFilePath.c_str(), &width, &height, &channels, 0);
    if (!pixelData) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    // Print out image width and height
    std::cout << "Image Width: " << width << std::endl;
    std::cout << "Image Height: " << height << std::endl;


    // Convert pixel data to binary
    std::vector<bool> binaryData;
    for (int i = 0; i < width * height * channels; ++i) {
        for (int j = 7; j >= 0; --j) {
            binaryData.push_back((pixelData[i] >> j) & 1);
        }
    }

    // Measure the time taken to encode the binary data
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<bool> encodedData = encodeMessage(binaryData, m);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Encoding took " << duration.count() << " seconds." << std::endl;

    // Introduce errors
    std::vector<bool> corruptedData = introduceErrors(encodedData, q);

    // Decode the data
    std::vector<bool> decodedData = decodeChunks(corruptedData, 1, m);

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
    size_t dotPos = outputFilename.find_last_of(".");
    if (dotPos != std::string::npos) {
        outputFilename.insert(dotPos, "_corrected");
    } else {
        outputFilename += "_corrected";
    }
    std::string outputFilePath = "photos/" + outputFilename;

    // Write corrected pixel data back to BMP file
    if (!stbi_write_bmp(outputFilePath.c_str(), width, height, channels, correctedPixelData.data())) {
        std::cerr << "Error: Unable to create output file" << std::endl;
        stbi_image_free(pixelData);
        return;
    }

    stbi_image_free(pixelData);
    std::cout << "Corrected image saved as " << outputFilename << std::endl;
}