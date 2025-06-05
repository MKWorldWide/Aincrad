#include "AssetManager.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <json/json.h>

namespace Aincrad {
namespace World {

AssetManager::AssetManager() {
    initializeAssetDatabase();
    initializeStreamingSystem();
    initializeMemoryManager();
}

AssetManager::~AssetManager() {
    cleanup();
}

void AssetManager::initializeAssetDatabase() {
    // Initialize asset database
    m_assetDatabase = std::make_unique<AssetDatabase>();
    
    // Load asset metadata
    loadAssetMetadata();
    
    // Initialize platform-specific settings
    initializePlatformSettings();
}

void AssetManager::initializeStreamingSystem() {
    // Initialize streaming system
    m_streamingSystem = std::make_unique<StreamingSystem>();
    
    // Configure streaming settings
    StreamingConfig config;
    config.enableStreaming = true;
    config.streamBufferSize = 1024 * 1024; // 1MB buffer
    config.streamDistance = 100.0f;
    config.strategy = StreamingStrategy::DistanceBased;
    
    m_streamingSystem->configure(config);
}

void AssetManager::initializeMemoryManager() {
    // Initialize memory manager
    m_memoryManager = std::make_unique<MemoryManager>();
    
    // Configure memory settings
    MemoryConfig config;
    config.maxMemoryUsage = 1024 * 1024 * 1024; // 1GB max memory
    config.enablePaging = true;
    config.strategy = PagingStrategy::PoolAllocation;
    
    m_memoryManager->configure(config);
}

void AssetManager::loadAssetMetadata() {
    // Load asset metadata from JSON file
    std::ifstream file("assets/metadata.json");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open asset metadata file");
    }
    
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(file, root)) {
        throw std::runtime_error("Failed to parse asset metadata");
    }
    
    // Process each asset entry
    for (const auto& asset : root["assets"]) {
        AssetMetadata metadata;
        metadata.assetId = asset["id"].asString();
        metadata.assetType = asset["type"].asString();
        
        // Process platforms
        for (const auto& platform : asset["platforms"]) {
            metadata.platforms.push_back(platform.asString());
        }
        
        // Process dependencies
        for (const auto& dependency : asset["dependencies"]) {
            metadata.dependencies.push_back(dependency.asString());
        }
        
        // Add metadata to database
        m_assetDatabase->addAssetMetadata(metadata);
    }
}

void AssetManager::initializePlatformSettings() {
    // Initialize platform-specific settings
    PlatformSpecificSettings settings;
    
    // Windows settings
    settings.windows.meshOptimization = true;
    settings.windows.shaderCompilation = true;
    settings.windows.textureCompression = true;
    settings.windows.memoryManagement = true;
    
    // Mac settings
    settings.mac.meshOptimization = true;
    settings.mac.shaderCompilation = true;
    settings.mac.textureCompression = true;
    settings.mac.memoryManagement = true;
    
    // Linux settings
    settings.linux.meshOptimization = true;
    settings.linux.shaderCompilation = true;
    settings.linux.textureCompression = true;
    settings.linux.memoryManagement = true;
    
    // VR settings
    settings.vr.motionControllerOptimization = true;
    settings.vr.roomScaleSupport = true;
    settings.vr.performanceOptimization = true;
    settings.vr.assetStreaming = true;
    
    m_platformSettings = settings;
}

std::shared_ptr<Asset> AssetManager::loadAsset(const std::string& assetId) {
    // Check if asset is already loaded
    auto it = m_loadedAssets.find(assetId);
    if (it != m_loadedAssets.end()) {
        return it->second;
    }
    
    // Get asset metadata
    auto metadata = m_assetDatabase->getAssetMetadata(assetId);
    if (!metadata) {
        throw std::runtime_error("Asset not found: " + assetId);
    }
    
    // Create asset loading configuration
    AssetLoadingConfig config;
    config.asyncLoading = true;
    config.streaming = true;
    config.priority = 1;
    config.strategy = LoadingStrategy::Streaming;
    config.platformSettings = m_platformSettings;
    
    // Load asset
    auto asset = std::make_shared<Asset>(metadata);
    asset->load(config);
    
    // Add to loaded assets
    m_loadedAssets[assetId] = asset;
    
    return asset;
}

void AssetManager::unloadAsset(const std::string& assetId) {
    auto it = m_loadedAssets.find(assetId);
    if (it != m_loadedAssets.end()) {
        it->second->unload();
        m_loadedAssets.erase(it);
    }
}

void AssetManager::update() {
    // Update streaming system
    m_streamingSystem->update();
    
    // Update memory manager
    m_memoryManager->update();
    
    // Update loaded assets
    for (auto& [id, asset] : m_loadedAssets) {
        asset->update();
    }
}

void AssetManager::cleanup() {
    // Unload all assets
    for (auto& [id, asset] : m_loadedAssets) {
        asset->unload();
    }
    m_loadedAssets.clear();
    
    // Cleanup systems
    m_streamingSystem.reset();
    m_memoryManager.reset();
    m_assetDatabase.reset();
}

} // namespace World
} // namespace Aincrad 