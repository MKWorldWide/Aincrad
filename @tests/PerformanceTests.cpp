#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include <random>
#include <vector>
#include <memory>

// Include Aincrad headers
#include "World/SharedAssets/AssetManager.h"
#include "World/ZoneSystem.h"
#include "Characters/PlayerCharacter.h"
#include "Combat/WeaponComponent.h"
#include "Skills/SkillSystemComponent.h"
#include "AI/EnemyAI.h"

namespace aincrad {
namespace tests {

class PerformanceTest : public ::testing::Test {
protected:
    void SetUp() override {
        m_assetManager = std::make_unique<World::AssetManager>();
        m_zoneSystem = std::make_unique<World::ZoneSystem>();
        m_player = std::make_unique<Characters::PlayerCharacter>();
        m_weapon = std::make_unique<Combat::WeaponComponent>();
        m_skillSystem = std::make_unique<Skills::SkillSystemComponent>();
    }

    void TearDown() override {
        m_assetManager.reset();
        m_zoneSystem.reset();
        m_player.reset();
        m_weapon.reset();
        m_skillSystem.reset();
    }

    // Helper function to measure execution time
    template<typename Func>
    auto measureTime(Func&& func) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    }

    // Helper function to generate random data
    std::vector<std::string> generateRandomAssetIds(size_t count) {
        std::vector<std::string> ids;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1000, 9999);

        for (size_t i = 0; i < count; ++i) {
            ids.push_back("asset_" + std::to_string(dis(gen)));
        }
        return ids;
    }

    std::unique_ptr<World::AssetManager> m_assetManager;
    std::unique_ptr<World::ZoneSystem> m_zoneSystem;
    std::unique_ptr<Characters::PlayerCharacter> m_player;
    std::unique_ptr<Combat::WeaponComponent> m_weapon;
    std::unique_ptr<Skills::SkillSystemComponent> m_skillSystem;
};

// Asset Management Performance Tests
TEST_F(PerformanceTest, AssetManager_BulkLoad_1000Assets) {
    const size_t assetCount = 1000;
    auto assetIds = generateRandomAssetIds(assetCount);

    auto duration = measureTime([&]() {
        for (const auto& id : assetIds) {
            m_assetManager->loadAsset(id);
        }
    });

    // Performance assertion: Should load 1000 assets in under 1 second
    EXPECT_LT(duration.count(), 1000000); // 1 second in microseconds
    std::cout << "Loaded " << assetCount << " assets in " 
              << duration.count() << " microseconds" << std::endl;
}

TEST_F(PerformanceTest, AssetManager_ConcurrentLoad_100Assets) {
    const size_t assetCount = 100;
    auto assetIds = generateRandomAssetIds(assetCount);

    auto duration = measureTime([&]() {
        std::vector<std::thread> threads;
        for (size_t i = 0; i < 4; ++i) {
            threads.emplace_back([&, i]() {
                size_t start = i * (assetCount / 4);
                size_t end = (i + 1) * (assetCount / 4);
                for (size_t j = start; j < end; ++j) {
                    m_assetManager->loadAsset(assetIds[j]);
                }
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }
    });

    // Performance assertion: Should handle concurrent loading efficiently
    EXPECT_LT(duration.count(), 500000); // 500ms in microseconds
    std::cout << "Concurrent load of " << assetCount << " assets in " 
              << duration.count() << " microseconds" << std::endl;
}

TEST_F(PerformanceTest, AssetManager_UpdateLoop_10000Iterations) {
    const size_t iterations = 10000;

    auto duration = measureTime([&]() {
        for (size_t i = 0; i < iterations; ++i) {
            m_assetManager->update();
        }
    });

    // Performance assertion: Should handle 10k updates in under 100ms
    EXPECT_LT(duration.count(), 100000); // 100ms in microseconds
    std::cout << "Asset manager update loop (" << iterations << " iterations) in " 
              << duration.count() << " microseconds" << std::endl;
}

// Zone System Performance Tests
TEST_F(PerformanceTest, ZoneSystem_ZoneTransition_1000Transitions) {
    const size_t transitions = 1000;

    auto duration = measureTime([&]() {
        for (size_t i = 0; i < transitions; ++i) {
            m_zoneSystem->transitionToZone("zone_" + std::to_string(i % 10));
        }
    });

    // Performance assertion: Should handle 1000 zone transitions in under 500ms
    EXPECT_LT(duration.count(), 500000); // 500ms in microseconds
    std::cout << "Zone transitions (" << transitions << ") in " 
              << duration.count() << " microseconds" << std::endl;
}

TEST_F(PerformanceTest, ZoneSystem_ZoneUpdate_10000Updates) {
    const size_t updates = 10000;

    auto duration = measureTime([&]() {
        for (size_t i = 0; i < updates; ++i) {
            m_zoneSystem->update();
        }
    });

    // Performance assertion: Should handle 10k zone updates in under 100ms
    EXPECT_LT(duration.count(), 100000); // 100ms in microseconds
    std::cout << "Zone system updates (" << updates << ") in " 
              << duration.count() << " microseconds" << std::endl;
}

// Combat System Performance Tests
TEST_F(PerformanceTest, CombatSystem_WeaponAttack_10000Attacks) {
    const size_t attacks = 10000;

    auto duration = measureTime([&]() {
        for (size_t i = 0; i < attacks; ++i) {
            m_weapon->performAttack();
        }
    });

    // Performance assertion: Should handle 10k weapon attacks in under 200ms
    EXPECT_LT(duration.count(), 200000); // 200ms in microseconds
    std::cout << "Weapon attacks (" << attacks << ") in " 
              << duration.count() << " microseconds" << std::endl;
}

TEST_F(PerformanceTest, CombatSystem_ComboChain_1000Combos) {
    const size_t combos = 1000;

    auto duration = measureTime([&]() {
        for (size_t i = 0; i < combos; ++i) {
            m_weapon->startCombo();
            m_weapon->addComboMove("slash");
            m_weapon->addComboMove("thrust");
            m_weapon->addComboMove("finisher");
            m_weapon->executeCombo();
        }
    });

    // Performance assertion: Should handle 1000 combo chains in under 300ms
    EXPECT_LT(duration.count(), 300000); // 300ms in microseconds
    std::cout << "Combo chains (" << combos << ") in " 
              << duration.count() << " microseconds" << std::endl;
}

// Skill System Performance Tests
TEST_F(PerformanceTest, SkillSystem_SkillActivation_10000Skills) {
    const size_t skills = 10000;

    // Add some test skills
    m_skillSystem->addSkill("Fireball", Skills::SkillType::Combat, 2.0f);
    m_skillSystem->addSkill("Heal", Skills::SkillType::Support, 5.0f);
    m_skillSystem->addSkill("Buff", Skills::SkillType::Support, 3.0f);

    auto duration = measureTime([&]() {
        for (size_t i = 0; i < skills; ++i) {
            m_skillSystem->activateSkill("Fireball");
            m_skillSystem->activateSkill("Heal");
            m_skillSystem->activateSkill("Buff");
        }
    });

    // Performance assertion: Should handle 30k skill activations in under 500ms
    EXPECT_LT(duration.count(), 500000); // 500ms in microseconds
    std::cout << "Skill activations (" << skills * 3 << ") in " 
              << duration.count() << " microseconds" << std::endl;
}

TEST_F(PerformanceTest, SkillSystem_SkillUpdate_10000Updates) {
    const size_t updates = 10000;

    auto duration = measureTime([&]() {
        for (size_t i = 0; i < updates; ++i) {
            m_skillSystem->update();
        }
    });

    // Performance assertion: Should handle 10k skill updates in under 100ms
    EXPECT_LT(duration.count(), 100000); // 100ms in microseconds
    std::cout << "Skill system updates (" << updates << ") in " 
              << duration.count() << " microseconds" << std::endl;
}

// Player Character Performance Tests
TEST_F(PerformanceTest, PlayerCharacter_Movement_10000Updates) {
    const size_t updates = 10000;

    auto duration = measureTime([&]() {
        for (size_t i = 0; i < updates; ++i) {
            m_player->updateMovement();
            m_player->updateStamina();
            m_player->updateHealth();
        }
    });

    // Performance assertion: Should handle 10k player updates in under 200ms
    EXPECT_LT(duration.count(), 200000); // 200ms in microseconds
    std::cout << "Player character updates (" << updates << ") in " 
              << duration.count() << " microseconds" << std::endl;
}

TEST_F(PerformanceTest, PlayerCharacter_CombatActions_10000Actions) {
    const size_t actions = 10000;

    auto duration = measureTime([&]() {
        for (size_t i = 0; i < actions; ++i) {
            m_player->performAttack();
            m_player->performBlock();
            m_player->performDodge();
        }
    });

    // Performance assertion: Should handle 30k combat actions in under 400ms
    EXPECT_LT(duration.count(), 400000); // 400ms in microseconds
    std::cout << "Player combat actions (" << actions * 3 << ") in " 
              << duration.count() << " microseconds" << std::endl;
}

// Memory Performance Tests
TEST_F(PerformanceTest, MemoryAllocation_10000Allocations) {
    const size_t allocations = 10000;
    std::vector<std::unique_ptr<World::Asset>> assets;

    auto duration = measureTime([&]() {
        for (size_t i = 0; i < allocations; ++i) {
            World::AssetMetadata metadata;
            metadata.assetId = "test_asset_" + std::to_string(i);
            metadata.assetType = "model";
            assets.push_back(std::make_unique<World::Asset>(metadata));
        }
    });

    // Performance assertion: Should handle 10k allocations in under 50ms
    EXPECT_LT(duration.count(), 50000); // 50ms in microseconds
    std::cout << "Memory allocations (" << allocations << ") in " 
              << duration.count() << " microseconds" << std::endl;
}

// VR-Specific Performance Tests
TEST_F(PerformanceTest, VR_HandTracking_10000Updates) {
    const size_t updates = 10000;

    auto duration = measureTime([&]() {
        for (size_t i = 0; i < updates; ++i) {
            // Simulate VR hand tracking updates
            // This would normally involve updating hand positions and gestures
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    });

    // Performance assertion: Should handle 10k VR updates in under 1 second
    EXPECT_LT(duration.count(), 1000000); // 1 second in microseconds
    std::cout << "VR hand tracking updates (" << updates << ") in " 
              << duration.count() << " microseconds" << std::endl;
}

TEST_F(PerformanceTest, VR_GestureRecognition_1000Gestures) {
    const size_t gestures = 1000;

    auto duration = measureTime([&]() {
        for (size_t i = 0; i < gestures; ++i) {
            // Simulate gesture recognition
            // This would normally involve analyzing hand positions and movements
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    });

    // Performance assertion: Should handle 1000 gesture recognitions in under 500ms
    EXPECT_LT(duration.count(), 500000); // 500ms in microseconds
    std::cout << "VR gesture recognition (" << gestures << ") in " 
              << duration.count() << " microseconds" << std::endl;
}

// Integration Performance Tests
TEST_F(PerformanceTest, FullGameLoop_1000Frames) {
    const size_t frames = 1000;

    auto duration = measureTime([&]() {
        for (size_t i = 0; i < frames; ++i) {
            // Simulate a full game frame
            m_assetManager->update();
            m_zoneSystem->update();
            m_player->updateMovement();
            m_skillSystem->update();
            m_weapon->update();
        }
    });

    // Performance assertion: Should handle 1000 game frames in under 1 second
    EXPECT_LT(duration.count(), 1000000); // 1 second in microseconds
    std::cout << "Full game loop (" << frames << " frames) in " 
              << duration.count() << " microseconds" << std::endl;
}

} // namespace tests
} // namespace aincrad

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    
    std::cout << "=== Aincrad Performance Test Suite ===" << std::endl;
    std::cout << "Running performance benchmarks..." << std::endl;
    
    return RUN_ALL_TESTS();
} 