#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Asset.h"

namespace Aincrad {
namespace World {

struct StreamingConfig {
    bool enableStreaming;
    int streamBufferSize;
    float streamDistance;
    enum class StreamingStrategy {
        DistanceBased,
        PriorityBased,
        QualityBased,
        PlatformSpecific
    } strategy;
    struct PlatformSpecificSettings {
        bool meshOptimization;
        bool shaderCompilation;
        bool textureCompression;
        bool memoryManagement;
    } platformSettings;
};

class StreamingSystem {
public:
    StreamingSystem();
    ~StreamingSystem();

    // Configuration
    void configure(const StreamingConfig& config);

    // Streaming management
    void startStreaming(const std::string& assetId);
    void stopStreaming(const std::string& assetId);
    void update();

private:
    StreamingConfig m_config;
    std::vector<std::string> m_streamingAssets;
};

} // namespace World
} // namespace Aincrad 