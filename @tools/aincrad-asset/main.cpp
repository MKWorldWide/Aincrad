#include <iostream>
#include <string>
#include <stdexcept>
#include <filesystem>
#include <cxxopts.hpp>
#include "import_model.cpp"
#include "import_texture.cpp"
#include "import_audio.cpp"
#include "export_model.cpp"
#include "export_texture.cpp"
#include "export_audio.cpp"

int main(int argc, char* argv[]) {
    cxxopts::Options options("aincrad-asset", "Aincrad Asset Management CLI Tool");
    options.add_options()
        ("h,help", "Show help")
        ("c,command", "Command to execute (import/export)", cxxopts::value<std::string>())
        ("t,type", "Asset type (model/texture/audio)", cxxopts::value<std::string>())
        ("i,input", "Input file path", cxxopts::value<std::string>())
        ("o,output", "Output file path", cxxopts::value<std::string>())
        ("p,platform", "Target platform (windows/mac/linux/vr)", cxxopts::value<std::string>());

    try {
        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }

        if (!result.count("command") || !result.count("type") || 
            !result.count("input") || !result.count("output") || 
            !result.count("platform")) {
            std::cerr << "Error: Missing required arguments" << std::endl;
            std::cout << options.help() << std::endl;
            return 1;
        }

        std::string command = result["command"].as<std::string>();
        std::string type = result["type"].as<std::string>();
        std::string inputFile = result["input"].as<std::string>();
        std::string outputFile = result["output"].as<std::string>();
        std::string platform = result["platform"].as<std::string>();

        // Validate input file exists
        if (!std::filesystem::exists(inputFile)) {
            std::cerr << "Error: Input file does not exist: " << inputFile << std::endl;
            return 1;
        }

        // Create output directory if it doesn't exist
        std::filesystem::path outputPath(outputFile);
        std::filesystem::create_directories(outputPath.parent_path());

        // Execute command
        if (command == "import") {
            if (type == "model") {
                importModel(inputFile, outputFile, platform);
            } else if (type == "texture") {
                importTexture(inputFile, outputFile, platform);
            } else if (type == "audio") {
                importAudio(inputFile, outputFile, platform);
            } else {
                std::cerr << "Error: Unsupported asset type: " << type << std::endl;
                return 1;
            }
        } else if (command == "export") {
            if (type == "model") {
                exportModel(inputFile, outputFile, platform);
            } else if (type == "texture") {
                exportTexture(inputFile, outputFile, platform);
            } else if (type == "audio") {
                exportAudio(inputFile, outputFile, platform);
            } else {
                std::cerr << "Error: Unsupported asset type: " << type << std::endl;
                return 1;
            }
        } else {
            std::cerr << "Error: Unsupported command: " << command << std::endl;
            return 1;
        }

    } catch (const cxxopts::OptionException& e) {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 