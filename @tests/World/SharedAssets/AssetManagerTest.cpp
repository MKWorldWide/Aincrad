#include <gtest/gtest.h>
#include "World/SharedAssets/AssetManager.h"

using namespace Aincrad::World;

class AssetManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        m_assetManager = std::make_unique<AssetManager>();
    }

    void TearDown() override {
        m_assetManager.reset();
    }

    std::unique_ptr<AssetManager> m_assetManager;
};

TEST_F(AssetManagerTest, LoadAsset) {
    // Create test asset metadata
    AssetMetadata metadata;
    metadata.assetId = "test_asset";
    metadata.assetType = "model";
    metadata.platforms = {"windows", "mac", "linux"};
    metadata.dependencies = {};
    metadata.version = "1.0.0";
    metadata.permissions = "public";
    metadata.usage = "test";

    // Add metadata to database
    m_assetManager->m_assetDatabase->addAssetMetadata(metadata);

    // Load asset
    auto asset = m_assetManager->loadAsset("test_asset");
    ASSERT_NE(asset, nullptr);
    EXPECT_EQ(asset->getMetadata().assetId, "test_asset");
    EXPECT_TRUE(asset->isLoaded());
}

TEST_F(AssetManagerTest, UnloadAsset) {
    // Create test asset metadata
    AssetMetadata metadata;
    metadata.assetId = "test_asset";
    metadata.assetType = "model";
    metadata.platforms = {"windows", "mac", "linux"};
    metadata.dependencies = {};
    metadata.version = "1.0.0";
    metadata.permissions = "public";
    metadata.usage = "test";

    // Add metadata to database
    m_assetManager->m_assetDatabase->addAssetMetadata(metadata);

    // Load asset
    auto asset = m_assetManager->loadAsset("test_asset");
    ASSERT_NE(asset, nullptr);
    EXPECT_TRUE(asset->isLoaded());

    // Unload asset
    m_assetManager->unloadAsset("test_asset");
    EXPECT_FALSE(asset->isLoaded());
}

TEST_F(AssetManagerTest, Update) {
    // Create test asset metadata
    AssetMetadata metadata;
    metadata.assetId = "test_asset";
    metadata.assetType = "model";
    metadata.platforms = {"windows", "mac", "linux"};
    metadata.dependencies = {};
    metadata.version = "1.0.0";
    metadata.permissions = "public";
    metadata.usage = "test";

    // Add metadata to database
    m_assetManager->m_assetDatabase->addAssetMetadata(metadata);

    // Load asset
    auto asset = m_assetManager->loadAsset("test_asset");
    ASSERT_NE(asset, nullptr);

    // Update asset manager
    m_assetManager->update();
    EXPECT_TRUE(asset->isLoaded());
}

TEST_F(AssetManagerTest, Cleanup) {
    // Create test asset metadata
    AssetMetadata metadata;
    metadata.assetId = "test_asset";
    metadata.assetType = "model";
    metadata.platforms = {"windows", "mac", "linux"};
    metadata.dependencies = {};
    metadata.version = "1.0.0";
    metadata.permissions = "public";
    metadata.usage = "test";

    // Add metadata to database
    m_assetManager->m_assetDatabase->addAssetMetadata(metadata);

    // Load asset
    auto asset = m_assetManager->loadAsset("test_asset");
    ASSERT_NE(asset, nullptr);
    EXPECT_TRUE(asset->isLoaded());

    // Cleanup asset manager
    m_assetManager->cleanup();
    EXPECT_FALSE(asset->isLoaded());
}

TEST_F(AssetManagerTest, InvalidAsset) {
    // Try to load non-existent asset
    EXPECT_THROW(m_assetManager->loadAsset("invalid_asset"), std::runtime_error);
}

TEST_F(AssetManagerTest, DuplicateAsset) {
    // Create test asset metadata
    AssetMetadata metadata;
    metadata.assetId = "test_asset";
    metadata.assetType = "model";
    metadata.platforms = {"windows", "mac", "linux"};
    metadata.dependencies = {};
    metadata.version = "1.0.0";
    metadata.permissions = "public";
    metadata.usage = "test";

    // Add metadata to database
    m_assetManager->m_assetDatabase->addAssetMetadata(metadata);

    // Try to add duplicate metadata
    EXPECT_THROW(m_assetManager->m_assetDatabase->addAssetMetadata(metadata), std::runtime_error);
}

TEST_F(AssetManagerTest, AssetDependencies) {
    // Create dependency asset metadata
    AssetMetadata depMetadata;
    depMetadata.assetId = "dependency_asset";
    depMetadata.assetType = "texture";
    depMetadata.platforms = {"windows", "mac", "linux"};
    depMetadata.dependencies = {};
    depMetadata.version = "1.0.0";
    depMetadata.permissions = "public";
    depMetadata.usage = "test";

    // Create test asset metadata with dependency
    AssetMetadata metadata;
    metadata.assetId = "test_asset";
    metadata.assetType = "model";
    metadata.platforms = {"windows", "mac", "linux"};
    metadata.dependencies = {"dependency_asset"};
    metadata.version = "1.0.0";
    metadata.permissions = "public";
    metadata.usage = "test";

    // Add metadata to database
    m_assetManager->m_assetDatabase->addAssetMetadata(depMetadata);
    m_assetManager->m_assetDatabase->addAssetMetadata(metadata);

    // Validate asset dependencies
    EXPECT_TRUE(m_assetManager->m_assetDatabase->validateAsset("test_asset"));
}

TEST_F(AssetManagerTest, InvalidDependencies) {
    // Create test asset metadata with invalid dependency
    AssetMetadata metadata;
    metadata.assetId = "test_asset";
    metadata.assetType = "model";
    metadata.platforms = {"windows", "mac", "linux"};
    metadata.dependencies = {"invalid_dependency"};
    metadata.version = "1.0.0";
    metadata.permissions = "public";
    metadata.usage = "test";

    // Add metadata to database
    m_assetManager->m_assetDatabase->addAssetMetadata(metadata);

    // Validate asset dependencies
    EXPECT_FALSE(m_assetManager->m_assetDatabase->validateAsset("test_asset"));
} 