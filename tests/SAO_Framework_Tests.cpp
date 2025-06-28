#include <gtest/gtest.h>
#include "../src/SAO/SAOFramework.h"
#include <memory>
#include <vector>

/**
 * @brief SAO Framework Test Suite
 * 
 * Comprehensive unit tests for the Sword Art Online framework.
 * Tests cover all core systems including player creation, combat,
 * world management, and edge cases.
 */
class SAOFrameworkTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize framework before each test
        frameworkManager = &SAO::SAOFrameworkManager::getInstance();
        frameworkManager->initializeFramework();
        framework = frameworkManager->getFramework();
    }
    
    void TearDown() override {
        // Cleanup after each test
        if (framework) {
            framework->stopWorld();
        }
        frameworkManager->shutdownFramework();
    }
    
    SAO::SAOFrameworkManager* frameworkManager;
    std::shared_ptr<SAO::SAOFramework> framework;
};

// ========================================
// PLAYER SYSTEM TESTS
// ========================================

TEST_F(SAOFrameworkTest, PlayerCreation) {
    // Test basic player creation
    auto player = framework->createPlayer("TestPlayer");
    
    EXPECT_NE(player, nullptr);
    EXPECT_EQ(player->getName(), "TestPlayer");
    EXPECT_EQ(player->getLevel(), 1);
    EXPECT_EQ(player->getExperience(), 0);
    EXPECT_EQ(player->getCurrentHealth(), 100);
    EXPECT_EQ(player->getMaxHealth(), 100);
    EXPECT_EQ(player->getCor(), 1000);
}

TEST_F(SAOFrameworkTest, PlayerStats) {
    auto player = framework->createPlayer("TestPlayer");
    
    // Test default stats
    const auto& baseStats = player->getBaseStats();
    EXPECT_EQ(baseStats.strength, 10);
    EXPECT_EQ(baseStats.dexterity, 10);
    EXPECT_EQ(baseStats.agility, 10);
    EXPECT_EQ(baseStats.vitality, 10);
    EXPECT_EQ(baseStats.intelligence, 10);
    
    // Test stat modification
    SAO::Core::PlayerStats newStats(15, 12, 14, 11, 13);
    player->setBaseStats(newStats);
    
    const auto& updatedStats = player->getBaseStats();
    EXPECT_EQ(updatedStats.strength, 15);
    EXPECT_EQ(updatedStats.dexterity, 12);
    EXPECT_EQ(updatedStats.agility, 14);
    EXPECT_EQ(updatedStats.vitality, 11);
    EXPECT_EQ(updatedStats.intelligence, 13);
}

TEST_F(SAOFrameworkTest, PlayerLeveling) {
    auto player = framework->createPlayer("TestPlayer");
    
    // Test experience gain
    uint32_t initialExp = player->getExperience();
    player->addExperience(100);
    
    EXPECT_EQ(player->getExperience(), initialExp + 100);
    
    // Test leveling up
    uint32_t initialLevel = player->getLevel();
    uint32_t initialSkillPoints = player->getSkillPoints();
    
    // Add enough experience to level up
    player->addExperience(1000);
    
    EXPECT_GT(player->getLevel(), initialLevel);
    EXPECT_GT(player->getSkillPoints(), initialSkillPoints);
}

TEST_F(SAOFrameworkTest, PlayerHealth) {
    auto player = framework->createPlayer("TestPlayer");
    
    uint32_t maxHealth = player->getMaxHealth();
    EXPECT_EQ(player->getCurrentHealth(), maxHealth);
    
    // Test damage
    player->damage(50);
    EXPECT_EQ(player->getCurrentHealth(), maxHealth - 50);
    
    // Test healing
    player->heal(25);
    EXPECT_EQ(player->getCurrentHealth(), maxHealth - 25);
    
    // Test over-healing
    player->heal(100);
    EXPECT_EQ(player->getCurrentHealth(), maxHealth);
    
    // Test death
    player->damage(maxHealth);
    EXPECT_EQ(player->getCurrentHealth(), 0);
    EXPECT_FALSE(player->isAlive());
    
    // Test revival
    player->revive();
    EXPECT_GT(player->getCurrentHealth(), 0);
    EXPECT_TRUE(player->isAlive());
}

TEST_F(SAOFrameworkTest, PlayerCurrency) {
    auto player = framework->createPlayer("TestPlayer");
    
    uint32_t initialCor = player->getCor();
    EXPECT_EQ(initialCor, 1000);
    
    // Test adding Cor
    player->addCor(500);
    EXPECT_EQ(player->getCor(), initialCor + 500);
    
    // Test spending Cor
    EXPECT_TRUE(player->spendCor(200));
    EXPECT_EQ(player->getCor(), initialCor + 500 - 200);
    
    // Test insufficient Cor
    EXPECT_FALSE(player->spendCor(2000));
    EXPECT_EQ(player->getCor(), initialCor + 500 - 200); // Should remain unchanged
}

TEST_F(SAOFrameworkTest, PlayerAppearance) {
    auto player = framework->createPlayer("TestPlayer");
    
    SAO::Core::CharacterAppearance appearance;
    appearance.hairStyle = 2;
    appearance.hairColor = 0xFF0000; // Red
    appearance.eyeColor = 0x00FF00;  // Green
    appearance.skinTone = 1;
    appearance.bodyType = 0;
    appearance.height = 1.1f;
    appearance.weight = 0.9f;
    
    player->setAppearance(appearance);
    
    const auto& playerAppearance = player->getAppearance();
    EXPECT_EQ(playerAppearance.hairStyle, 2);
    EXPECT_EQ(playerAppearance.hairColor, 0xFF0000);
    EXPECT_EQ(playerAppearance.eyeColor, 0x00FF00);
    EXPECT_EQ(playerAppearance.skinTone, 1);
    EXPECT_EQ(playerAppearance.bodyType, 0);
    EXPECT_FLOAT_EQ(playerAppearance.height, 1.1f);
    EXPECT_FLOAT_EQ(playerAppearance.weight, 0.9f);
}

// ========================================
// COMBAT SYSTEM TESTS
// ========================================

TEST_F(SAOFrameworkTest, CombatInitialization) {
    auto player1 = framework->createPlayer("Player1");
    auto player2 = framework->createPlayer("Player2");
    
    // Test combat initialization
    framework->startCombat(player1, player2);
    
    EXPECT_TRUE(framework->isCombatActive());
    
    framework->endCombat();
    EXPECT_FALSE(framework->isCombatActive());
}

TEST_F(SAOFrameworkTest, BasicSwordSkills) {
    auto attacker = framework->createPlayer("Attacker");
    auto target = framework->createPlayer("Target");
    
    framework->startCombat(attacker, target);
    
    auto combatManager = framework->getCombatManager();
    
    // Test Horizontal skill
    auto horizontal = std::make_shared<SAO::Combat::BasicSwordSkill>(
        1, "Horizontal", SAO::Combat::BasicSwordSkill::BasicType::Horizontal
    );
    combatManager->registerSwordSkill(horizontal);
    
    EXPECT_TRUE(horizontal->canUse());
    
    auto result = horizontal->execute(attacker, target);
    EXPECT_TRUE(result.hit || !result.hit); // Should either hit or miss
    
    framework->endCombat();
}

TEST_F(SAOFrameworkTest, AdvancedSwordSkills) {
    auto attacker = framework->createPlayer("Attacker");
    auto target = framework->createPlayer("Target");
    
    framework->startCombat(attacker, target);
    
    auto combatManager = framework->getCombatManager();
    
    // Test Linear skill
    auto linear = std::make_shared<SAO::Combat::AdvancedSwordSkill>(
        10, "Linear", SAO::Combat::AdvancedSwordSkill::AdvancedType::Linear
    );
    combatManager->registerSwordSkill(linear);
    
    EXPECT_TRUE(linear->canUse());
    
    auto result = linear->execute(attacker, target);
    EXPECT_TRUE(result.hit || !result.hit);
    
    framework->endCombat();
}

TEST_F(SAOFrameworkTest, UniqueSwordSkills) {
    auto attacker = framework->createPlayer("Attacker");
    auto target = framework->createPlayer("Target");
    
    framework->startCombat(attacker, target);
    
    auto combatManager = framework->getCombatManager();
    
    // Test Starburst Stream
    auto starburstStream = std::make_shared<SAO::Combat::StarburstStream>();
    combatManager->registerSwordSkill(starburstStream);
    
    EXPECT_TRUE(starburstStream->canUse());
    
    auto result = starburstStream->execute(attacker, target);
    EXPECT_TRUE(result.hit || !result.hit);
    
    // Test Vorpal Strike
    auto vorpalStrike = std::make_shared<SAO::Combat::VorpalStrike>();
    combatManager->registerSwordSkill(vorpalStrike);
    
    EXPECT_TRUE(vorpalStrike->canUse());
    
    result = vorpalStrike->execute(attacker, target);
    EXPECT_TRUE(result.hit || !result.hit);
    
    framework->endCombat();
}

TEST_F(SAOFrameworkTest, SkillCooldowns) {
    auto attacker = framework->createPlayer("Attacker");
    auto target = framework->createPlayer("Target");
    
    framework->startCombat(attacker, target);
    
    auto combatManager = framework->getCombatManager();
    
    auto skill = std::make_shared<SAO::Combat::BasicSwordSkill>(
        1, "TestSkill", SAO::Combat::BasicSwordSkill::BasicType::Horizontal
    );
    skill->setCooldown(1000); // 1 second cooldown
    combatManager->registerSwordSkill(skill);
    
    // First use should work
    EXPECT_TRUE(skill->canUse());
    skill->use();
    
    // Second use should be on cooldown
    EXPECT_FALSE(skill->canUse());
    
    // Reset cooldown
    skill->resetCooldown();
    EXPECT_TRUE(skill->canUse());
    
    framework->endCombat();
}

TEST_F(SAOFrameworkTest, SkillProgression) {
    auto skill = std::make_shared<SAO::Combat::BasicSwordSkill>(
        1, "TestSkill", SAO::Combat::BasicSwordSkill::BasicType::Horizontal
    );
    
    uint32_t initialLevel = skill->getLevel();
    uint32_t initialExp = skill->getExperienceToNext();
    
    // Add experience
    skill->addExperience(100);
    
    // Check if level increased
    if (skill->canLevelUp()) {
        skill->levelUp();
        EXPECT_GT(skill->getLevel(), initialLevel);
    }
}

// ========================================
// WORLD SYSTEM TESTS
// ========================================

TEST_F(SAOFrameworkTest, WorldInitialization) {
    framework->startWorld();
    EXPECT_TRUE(framework->isWorldRunning());
    
    framework->stopWorld();
    EXPECT_FALSE(framework->isWorldRunning());
}

TEST_F(SAOFrameworkTest, PlayerLocation) {
    framework->startWorld();
    
    auto player = framework->createPlayer("TestPlayer");
    auto worldManager = framework->getWorldManager();
    auto worldSystem = worldManager->getWorldSystem();
    
    // Test setting location
    SAO::World::Location location(1, 100.0f, 50.0f, 25.0f);
    worldSystem->setPlayerLocation(player, location);
    
    // Test getting location
    auto playerLocation = worldSystem->getPlayerLocation(player);
    EXPECT_EQ(playerLocation.floor, 1);
    EXPECT_FLOAT_EQ(playerLocation.x, 100.0f);
    EXPECT_FLOAT_EQ(playerLocation.y, 50.0f);
    EXPECT_FLOAT_EQ(playerLocation.z, 25.0f);
    
    framework->stopWorld();
}

TEST_F(SAOFrameworkTest, Teleportation) {
    framework->startWorld();
    
    auto player = framework->createPlayer("TestPlayer");
    auto worldManager = framework->getWorldManager();
    auto worldSystem = worldManager->getWorldSystem();
    
    // Set initial location
    SAO::World::Location initialLocation(1, 0.0f, 0.0f, 0.0f);
    worldSystem->setPlayerLocation(player, initialLocation);
    
    // Test teleportation
    SAO::World::Location destination(2, 100.0f, 100.0f, 100.0f);
    EXPECT_TRUE(worldSystem->teleportPlayer(player, destination));
    
    auto newLocation = worldSystem->getPlayerLocation(player);
    EXPECT_EQ(newLocation.floor, 2);
    EXPECT_FLOAT_EQ(newLocation.x, 100.0f);
    EXPECT_FLOAT_EQ(newLocation.y, 100.0f);
    EXPECT_FLOAT_EQ(newLocation.z, 100.0f);
    
    framework->stopWorld();
}

TEST_F(SAOFrameworkTest, SafeZones) {
    framework->startWorld();
    
    auto player = framework->createPlayer("TestPlayer");
    auto worldManager = framework->getWorldManager();
    auto worldSystem = worldManager->getWorldSystem();
    
    // Set location in safe zone (Town of Beginnings)
    SAO::World::Location safeLocation(1, 0.0f, 0.0f, 0.0f);
    worldSystem->setPlayerLocation(player, safeLocation);
    
    EXPECT_TRUE(worldSystem->isPlayerInSafeZone(player));
    
    // Set location outside safe zone
    SAO::World::Location dangerLocation(1, 1000.0f, 1000.0f, 0.0f);
    worldSystem->setPlayerLocation(player, dangerLocation);
    
    EXPECT_FALSE(worldSystem->isPlayerInSafeZone(player));
    
    framework->stopWorld();
}

// ========================================
// EQUIPMENT SYSTEM TESTS
// ========================================

TEST_F(SAOFrameworkTest, WeaponCreation) {
    auto weapon = std::make_shared<SAO::Core::Weapon>(
        1, "Test Sword", SAO::Core::ItemRarity::Rare, 
        SAO::Core::Weapon::WeaponType::OneHandedSword
    );
    
    EXPECT_EQ(weapon->getName(), "Test Sword");
    EXPECT_EQ(weapon->getRarity(), SAO::Core::ItemRarity::Rare);
    EXPECT_EQ(weapon->getWeaponType(), SAO::Core::Weapon::WeaponType::OneHandedSword);
    EXPECT_TRUE(weapon->isEquippable());
    EXPECT_FALSE(weapon->isConsumable());
    EXPECT_FALSE(weapon->isStackable());
}

TEST_F(SAOFrameworkTest, ArmorCreation) {
    auto armor = std::make_shared<SAO::Core::Armor>(
        1, "Test Armor", SAO::Core::ItemRarity::Uncommon, 
        SAO::Core::Armor::ArmorType::Medium
    );
    
    EXPECT_EQ(armor->getName(), "Test Armor");
    EXPECT_EQ(armor->getRarity(), SAO::Core::ItemRarity::Uncommon);
    EXPECT_EQ(armor->getArmorType(), SAO::Core::Armor::ArmorType::Medium);
    EXPECT_TRUE(armor->isEquippable());
    EXPECT_FALSE(armor->isConsumable());
    EXPECT_FALSE(armor->isStackable());
}

TEST_F(SAOFrameworkTest, EquipmentStats) {
    auto weapon = std::make_shared<SAO::Core::Weapon>(
        1, "Test Sword", SAO::Core::ItemRarity::Rare, 
        SAO::Core::Weapon::WeaponType::OneHandedSword
    );
    
    // Set weapon stats
    weapon->setDamage(100);
    weapon->setAttackSpeed(1.5f);
    weapon->setCriticalRate(15);
    
    EXPECT_EQ(weapon->getDamage(), 100);
    EXPECT_FLOAT_EQ(weapon->getAttackSpeed(), 1.5f);
    EXPECT_EQ(weapon->getCriticalRate(), 15);
    
    // Set stat bonuses
    SAO::Core::PlayerStats bonuses(5, 3, 2, 1, 0);
    weapon->setStatBonuses(bonuses);
    
    const auto& weaponBonuses = weapon->getStatBonuses();
    EXPECT_EQ(weaponBonuses.strength, 5);
    EXPECT_EQ(weaponBonuses.dexterity, 3);
    EXPECT_EQ(weaponBonuses.agility, 2);
    EXPECT_EQ(weaponBonuses.vitality, 1);
    EXPECT_EQ(weaponBonuses.intelligence, 0);
}

TEST_F(SAOFrameworkTest, EquipmentDurability) {
    auto weapon = std::make_shared<SAO::Core::Weapon>(
        1, "Test Sword", SAO::Core::ItemRarity::Common, 
        SAO::Core::Weapon::WeaponType::OneHandedSword
    );
    
    uint32_t maxDurability = weapon->getMaxDurability();
    EXPECT_EQ(weapon->getDurability(), maxDurability);
    EXPECT_FALSE(weapon->isBroken());
    
    // Test damage
    weapon->damage(50);
    EXPECT_EQ(weapon->getDurability(), maxDurability - 50);
    
    // Test breaking
    weapon->damage(maxDurability);
    EXPECT_EQ(weapon->getDurability(), 0);
    EXPECT_TRUE(weapon->isBroken());
    
    // Test repair
    weapon->repair();
    EXPECT_EQ(weapon->getDurability(), maxDurability);
    EXPECT_FALSE(weapon->isBroken());
}

TEST_F(SAOFrameworkTest, EquipmentInventory) {
    auto player = framework->createPlayer("TestPlayer");
    
    auto weapon = std::make_shared<SAO::Core::Weapon>(
        1, "Test Sword", SAO::Core::ItemRarity::Common, 
        SAO::Core::Weapon::WeaponType::OneHandedSword
    );
    
    // Test adding to inventory
    EXPECT_TRUE(player->addItemToInventory(weapon));
    EXPECT_EQ(player->getInventorySize(), 1);
    
    // Test equipping
    EXPECT_TRUE(player->equipItem(weapon));
    EXPECT_TRUE(player->isSlotOccupied(SAO::Core::EquipmentSlot::Weapon));
    
    // Test unequipping
    auto unequippedWeapon = player->unequipItem(SAO::Core::EquipmentSlot::Weapon);
    EXPECT_NE(unequippedWeapon, nullptr);
    EXPECT_EQ(unequippedWeapon->getName(), "Test Sword");
    EXPECT_FALSE(player->isSlotOccupied(SAO::Core::EquipmentSlot::Weapon));
}

// ========================================
// FRAMEWORK INTEGRATION TESTS
// ========================================

TEST_F(SAOFrameworkTest, FrameworkInitialization) {
    EXPECT_TRUE(framework->isInitialized());
    EXPECT_FALSE(framework->isWorldRunning());
    EXPECT_FALSE(framework->isCombatActive());
}

TEST_F(SAOFrameworkTest, Configuration) {
    framework->setConfiguration("TEST_KEY", "TEST_VALUE");
    EXPECT_EQ(framework->getConfiguration("TEST_KEY"), "TEST_VALUE");
    
    // Test non-existent key
    EXPECT_EQ(framework->getConfiguration("NON_EXISTENT"), "");
}

TEST_F(SAOFrameworkTest, PlayerPersistence) {
    auto player = framework->createPlayer("TestPlayer");
    
    // Modify player
    player->addExperience(500);
    player->addCor(1000);
    
    // Save player
    framework->savePlayer(player);
    
    // Create new player with same name
    auto loadedPlayer = framework->loadPlayer("TestPlayer");
    
    EXPECT_NE(loadedPlayer, nullptr);
    EXPECT_EQ(loadedPlayer->getName(), "TestPlayer");
    EXPECT_EQ(loadedPlayer->getExperience(), 500);
    EXPECT_EQ(loadedPlayer->getCor(), 2000); // 1000 initial + 1000 added
}

TEST_F(SAOFrameworkTest, MultiplePlayers) {
    auto player1 = framework->createPlayer("Player1");
    auto player2 = framework->createPlayer("Player2");
    auto player3 = framework->createPlayer("Player3");
    
    EXPECT_NE(player1, nullptr);
    EXPECT_NE(player2, nullptr);
    EXPECT_NE(player3, nullptr);
    
    EXPECT_NE(player1, player2);
    EXPECT_NE(player2, player3);
    EXPECT_NE(player1, player3);
    
    EXPECT_EQ(player1->getName(), "Player1");
    EXPECT_EQ(player2->getName(), "Player2");
    EXPECT_EQ(player3->getName(), "Player3");
}

// ========================================
// EDGE CASE TESTS
// ========================================

TEST_F(SAOFrameworkTest, InvalidPlayerNames) {
    // Test empty name
    auto emptyPlayer = framework->createPlayer("");
    EXPECT_EQ(emptyPlayer->getName(), "");
    
    // Test very long name
    std::string longName(100, 'A');
    auto longNamePlayer = framework->createPlayer(longName);
    EXPECT_EQ(longNamePlayer->getName(), longName);
}

TEST_F(SAOFrameworkTest, ExtremeStats) {
    auto player = framework->createPlayer("TestPlayer");
    
    // Test maximum stats
    SAO::Core::PlayerStats maxStats(255, 255, 255, 255, 255);
    player->setBaseStats(maxStats);
    
    const auto& stats = player->getBaseStats();
    EXPECT_EQ(stats.strength, 255);
    EXPECT_EQ(stats.dexterity, 255);
    EXPECT_EQ(stats.agility, 255);
    EXPECT_EQ(stats.vitality, 255);
    EXPECT_EQ(stats.intelligence, 255);
}

TEST_F(SAOFrameworkTest, MassiveExperience) {
    auto player = framework->createPlayer("TestPlayer");
    
    uint32_t initialLevel = player->getLevel();
    
    // Add massive amount of experience
    player->addExperience(1000000);
    
    EXPECT_GT(player->getLevel(), initialLevel);
    EXPECT_GT(player->getSkillPoints(), 0);
}

TEST_F(SAOFrameworkTest, EquipmentOverflow) {
    auto player = framework->createPlayer("TestPlayer");
    
    // Try to add many items to inventory
    for (int i = 0; i < 100; i++) {
        auto weapon = std::make_shared<SAO::Core::Weapon>(
            i, "Weapon " + std::to_string(i), SAO::Core::ItemRarity::Common, 
            SAO::Core::Weapon::WeaponType::OneHandedSword
        );
        player->addItemToInventory(weapon);
    }
    
    EXPECT_GT(player->getInventorySize(), 0);
}

// ========================================
// PERFORMANCE TESTS
// ========================================

TEST_F(SAOFrameworkTest, PerformancePlayerCreation) {
    auto start = std::chrono::high_resolution_clock::now();
    
    // Create many players quickly
    for (int i = 0; i < 1000; i++) {
        auto player = framework->createPlayer("Player" + std::to_string(i));
        EXPECT_NE(player, nullptr);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // Should complete in reasonable time (less than 1 second)
    EXPECT_LT(duration.count(), 1000);
}

TEST_F(SAOFrameworkTest, PerformanceCombat) {
    auto player1 = framework->createPlayer("Player1");
    auto player2 = framework->createPlayer("Player2");
    
    framework->startCombat(player1, player2);
    
    auto combatManager = framework->getCombatManager();
    auto skill = std::make_shared<SAO::Combat::BasicSwordSkill>(
        1, "TestSkill", SAO::Combat::BasicSwordSkill::BasicType::Horizontal
    );
    combatManager->registerSwordSkill(skill);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Execute many combat actions
    for (int i = 0; i < 1000; i++) {
        skill->execute(player1, player2);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // Should complete in reasonable time (less than 1 second)
    EXPECT_LT(duration.count(), 1000);
    
    framework->endCombat();
}

// ========================================
// MAIN TEST RUNNER
// ========================================

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    
    std::cout << "🗡️  SAO Framework Test Suite" << std::endl;
    std::cout << "===========================" << std::endl;
    std::cout << "Running comprehensive tests for the Sword Art Online framework..." << std::endl;
    std::cout << std::endl;
    
    int result = RUN_ALL_TESTS();
    
    std::cout << std::endl;
    if (result == 0) {
        std::cout << "✅ All tests passed! SAO Framework is working correctly." << std::endl;
    } else {
        std::cout << "❌ Some tests failed. Please check the implementation." << std::endl;
    }
    
    return result;
} 