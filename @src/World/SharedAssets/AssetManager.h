#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "Asset.h"
#include "AssetDatabase.h"
#include "StreamingSystem.h"
#include "MemoryManager.h"

namespace Aincrad {
namespace World {

class AssetManager {
public:
    AssetManager();
    ~AssetManager();

    // Asset loading and unloading
    std::shared_ptr<Asset> loadAsset(const std::string& assetId);
    void unloadAsset(const std::string& assetId);

    // Update and cleanup
    void update();
    void cleanup();

private:
    // Initialization
    void initializeAssetDatabase();
    void initializeStreamingSystem();
    void initializeMemoryManager();
    void loadAssetMetadata();
    void initializePlatformSettings();

    // Systems
    std::unique_ptr<AssetDatabase> m_assetDatabase;
    std::unique_ptr<StreamingSystem> m_streamingSystem;
    std::unique_ptr<MemoryManager> m_memoryManager;

    // Asset storage
    std::unordered_map<std::string, std::shared_ptr<Asset>> m_loadedAssets;

    // Platform settings
    PlatformSpecificSettings m_platformSettings;
};

} // namespace World
} // namespace Aincrad 