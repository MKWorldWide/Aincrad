#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include "import_model.cpp"
#include "import_texture.cpp"
#include "import_audio.cpp"

void printHelp() {
    std::cout << "Usage: aincrad-asset [command] [options] <input_file> [output_file]" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  import: Convert external assets to internal format" << std::endl;
    std::cout << "  export: Convert internal assets to external format" << std::endl;
    std::cout << "  validate: Validate asset integrity and metadata" << std::endl;
    std::cout << "  info: Display asset metadata and dependencies" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --type <type>: Specify asset type (model, texture, audio)" << std::endl;
    std::cout << "  --platform <platform>: Target platform (windows, mac, linux, vr)" << std::endl;
    std::cout << "  --quality <quality>: Quality level (low, medium, high)" << std::endl;
    std::cout << "  --verbose: Enable verbose output" << std::endl;
    std::cout << "  --help: Display this help message" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printHelp();
        return 1;
    }

    std::string command = argv[1];
    std::vector<std::string> options;
    std::string inputFile;
    std::string outputFile;
    std::string type;
    std::string platform;

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            printHelp();
            return 0;
        } else if (strncmp(argv[i], "--", 2) == 0) {
            options.push_back(argv[i]);
            if (strcmp(argv[i], "--type") == 0 && i + 1 < argc) {
                type = argv[++i];
            } else if (strcmp(argv[i], "--platform") == 0 && i + 1 < argc) {
                platform = argv[++i];
            }
        } else if (inputFile.empty()) {
            inputFile = argv[i];
        } else if (outputFile.empty()) {
            outputFile = argv[i];
        }
    }

    if (command == "import") {
        if (type == "model") {
            importModel(inputFile, outputFile, platform);
        } else if (type == "texture") {
            importTexture(inputFile, outputFile, platform);
        } else if (type == "audio") {
            importAudio(inputFile, outputFile, platform);
        } else {
            std::cerr << "Unknown asset type: " << type << std::endl;
            printHelp();
            return 1;
        }
    } else if (command == "export") {
        std::cout << "Exporting " << inputFile << " to " << outputFile << std::endl;
        // TODO: Implement export logic
    } else if (command == "validate") {
        std::cout << "Validating " << inputFile << std::endl;
        // TODO: Implement validation logic
    } else if (command == "info") {
        std::cout << "Displaying info for " << inputFile << std::endl;
        // TODO: Implement info logic
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        printHelp();
        return 1;
    }

    return 0;
} 