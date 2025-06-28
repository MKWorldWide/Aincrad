#include "AssetDatabase.h"
#include <stdexcept>

namespace Aincrad {
namespace World {

AssetDatabase::AssetDatabase() {
}

AssetDatabase::~AssetDatabase() {
    m_metadata.clear();
}

void AssetDatabase::addAssetMetadata(const AssetMetadata& metadata) {
    auto it = m_metadata.find(metadata.assetId);
    if (it != m_metadata.end()) {
        throw std::runtime_error("Asset metadata already exists: " + metadata.assetId);
    }

    m_metadata[metadata.assetId] = std::make_shared<AssetMetadata>(metadata);
}

std::shared_ptr<AssetMetadata> AssetDatabase::getAssetMetadata(const std::string& assetId) {
    auto it = m_metadata.find(assetId);
    if (it == m_metadata.end()) {
        return nullptr;
    }

    return it->second;
}

void AssetDatabase::removeAssetMetadata(const std::string& assetId) {
    auto it = m_metadata.find(assetId);
    if (it != m_metadata.end()) {
        m_metadata.erase(it);
    }
}

bool AssetDatabase::validateAsset(const std::string& assetId) {
    auto metadata = getAssetMetadata(assetId);
    if (!metadata) {
        return false;
    }

    // Validate asset metadata
    if (metadata->assetId.empty() || metadata->assetType.empty()) {
        return false;
    }

    // Check dependencies
    return checkDependencies(assetId);
}

bool AssetDatabase::checkDependencies(const std::string& assetId) {
    auto metadata = getAssetMetadata(assetId);
    if (!metadata) {
        return false;
    }

    // Check each dependency
    for (const auto& dependency : metadata->dependencies) {
        auto depMetadata = getAssetMetadata(dependency);
        if (!depMetadata) {
            return false;
        }
    }

    return true;
}

} // namespace World
} // namespace Aincrad 