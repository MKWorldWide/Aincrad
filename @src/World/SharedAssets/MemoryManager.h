#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "Asset.h"

namespace Aincrad {
namespace World {

struct MemoryConfig {
    int maxMemoryUsage;
    bool enablePaging;
    enum class PagingStrategy {
        PoolAllocation,
        DynamicAllocation,
        PlatformSpecific,
        QualityBased
    } strategy;
    struct PlatformSpecificSettings {
        bool meshOptimization;
        bool shaderCompilation;
        bool textureCompression;
        bool memoryManagement;
    } platformSettings;
};

class MemoryManager {
public:
    MemoryManager();
    ~MemoryManager();

    // Configuration
    void configure(const MemoryConfig& config);

    // Memory management
    void allocateMemory(const std::string& assetId, size_t size);
    void deallocateMemory(const std::string& assetId);
    void update();

private:
    MemoryConfig m_config;
    std::unordered_map<std::string, size_t> m_allocatedMemory;
    size_t m_totalAllocated;
};

} // namespace World
} // namespace Aincrad 