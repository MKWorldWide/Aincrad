#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "Asset.h"

namespace Aincrad {
namespace World {

class AssetDatabase {
public:
    AssetDatabase();
    ~AssetDatabase();

    // Asset metadata management
    void addAssetMetadata(const AssetMetadata& metadata);
    std::shared_ptr<AssetMetadata> getAssetMetadata(const std::string& assetId);
    void removeAssetMetadata(const std::string& assetId);

    // Asset validation
    bool validateAsset(const std::string& assetId);
    bool checkDependencies(const std::string& assetId);

private:
    std::unordered_map<std::string, std::shared_ptr<AssetMetadata>> m_metadata;
};

} // namespace World
} // namespace Aincrad 