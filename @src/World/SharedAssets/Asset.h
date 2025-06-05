#pragma once

#include <string>
#include <vector>
#include <memory>

namespace Aincrad {
namespace World {

struct AssetMetadata {
    std::string assetId;
    std::string assetType;
    std::vector<std::string> platforms;
    std::vector<std::string> dependencies;
    std::string version;
    std::string permissions;
    std::string usage;
};

struct AssetLoadingConfig {
    bool asyncLoading;
    bool streaming;
    int priority;
    enum class LoadingStrategy {
        Streaming,
        Immediate,
        Background
    } strategy;
    struct PlatformSpecificSettings {
        bool meshOptimization;
        bool shaderCompilation;
        bool textureCompression;
        bool memoryManagement;
    } platformSettings;
};

class Asset {
public:
    Asset(const AssetMetadata& metadata);
    ~Asset();

    // Asset lifecycle
    void load(const AssetLoadingConfig& config);
    void unload();
    void update();

    // Getters
    const AssetMetadata& getMetadata() const { return m_metadata; }
    bool isLoaded() const { return m_isLoaded; }

private:
    AssetMetadata m_metadata;
    bool m_isLoaded;
    AssetLoadingConfig m_config;
};

} // namespace World
} // namespace Aincrad 