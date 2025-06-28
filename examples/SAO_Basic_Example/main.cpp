#include "../../src/SAO/SAOFramework.h"
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

/**
 * @brief SAO Framework Basic Example
 * 
 * This example demonstrates the core features of the SAO Framework:
 * - Character creation and customization
 * - Combat system with sword skills
 * - World navigation and teleportation
 * - Character progression and leveling
 * - Equipment and inventory management
 * - Social features (party system)
 */
int main() {
    std::cout << "🗡️  SAO Framework - Basic Example" << std::endl;
    std::cout << "=================================" << std::endl;
    std::cout << std::endl;
    
    try {
        // Initialize the SAO framework
        std::cout << "🚀 Initializing SAO Framework..." << std::endl;
        auto& frameworkManager = SAO::SAOFrameworkManager::getInstance();
        
        if (!frameworkManager.initializeFramework()) {
            std::cerr << "❌ Failed to initialize SAO Framework!" << std::endl;
            return 1;
        }
        
        auto framework = frameworkManager.getFramework();
        std::cout << "✅ SAO Framework initialized successfully!" << std::endl;
        std::cout << "   Version: " << framework->getFrameworkVersion() << std::endl;
        std::cout << std::endl;
        
        // ========================================
        // CHARACTER CREATION DEMONSTRATION
        // ========================================
        std::cout << "👤 CHARACTER CREATION" << std::endl;
        std::cout << "=====================" << std::endl;
        
        // Create Kirito (Dual Wielder)
        std::cout << "Creating Kirito (Dual Wielder)..." << std::endl;
        auto kirito = framework->createPlayer("Kirito");
        
        SAO::Core::CharacterAppearance kiritoAppearance;
        kiritoAppearance.hairStyle = 1;      // Spiky black hair
        kiritoAppearance.hairColor = 0x000000; // Black
        kiritoAppearance.eyeColor = 0x000000;  // Black eyes
        kiritoAppearance.skinTone = 1;       // Fair skin
        kiritoAppearance.bodyType = 0;       // Average build
        kiritoAppearance.height = 1.0f;      // Normal height
        kiritoAppearance.weight = 1.0f;      // Normal weight
        
        SAO::Core::PlayerStats kiritoStats(16, 15, 13, 10, 8); // High STR/DEX for dual wielding
        kirito->setAppearance(kiritoAppearance);
        kirito->setBaseStats(kiritoStats);
        
        std::cout << "✅ Kirito created!" << std::endl;
        std::cout << "   Level: " << kirito->getLevel() << std::endl;
        std::cout << "   Health: " << kirito->getCurrentHealth() << "/" << kirito->getMaxHealth() << std::endl;
        std::cout << "   Cor: " << kirito->getCor() << std::endl;
        std::cout << std::endl;
        
        // Create Asuna (Rapier User)
        std::cout << "Creating Asuna (Rapier User)..." << std::endl;
        auto asuna = framework->createPlayer("Asuna");
        
        SAO::Core::CharacterAppearance asunaAppearance;
        asunaAppearance.hairStyle = 2;       // Long hair
        asunaAppearance.hairColor = 0xFFD700; // Golden blonde
        asunaAppearance.eyeColor = 0x8B4513;  // Brown eyes
        asunaAppearance.skinTone = 0;        // Fair skin
        asunaAppearance.bodyType = 1;        // Slender build
        asunaAppearance.height = 0.95f;      // Slightly shorter
        asunaAppearance.weight = 0.9f;       // Slightly lighter
        
        SAO::Core::PlayerStats asunaStats(10, 18, 16, 8, 12); // High DEX/AGI for rapier
        asuna->setAppearance(asunaAppearance);
        asuna->setBaseStats(asunaStats);
        
        std::cout << "✅ Asuna created!" << std::endl;
        std::cout << "   Level: " << asuna->getLevel() << std::endl;
        std::cout << "   Health: " << asuna->getCurrentHealth() << "/" << asuna->getMaxHealth() << std::endl;
        std::cout << "   Cor: " << asuna->getCor() << std::endl;
        std::cout << std::endl;
        
        // Create Klein (Guild Leader)
        std::cout << "Creating Klein (Guild Leader)..." << std::endl;
        auto klein = framework->createPlayer("Klein");
        
        SAO::Core::CharacterAppearance kleinAppearance;
        kleinAppearance.hairStyle = 3;       // Short hair
        kleinAppearance.hairColor = 0x8B4513; // Brown
        kleinAppearance.eyeColor = 0x8B4513;  // Brown eyes
        kleinAppearance.skinTone = 1;        // Fair skin
        kleinAppearance.bodyType = 0;        // Average build
        kleinAppearance.height = 1.05f;      // Slightly taller
        kleinAppearance.weight = 1.1f;       // Slightly heavier
        
        SAO::Core::PlayerStats kleinStats(14, 12, 11, 12, 10); // Balanced stats
        klein->setAppearance(kleinAppearance);
        klein->setBaseStats(kleinStats);
        
        std::cout << "✅ Klein created!" << std::endl;
        std::cout << "   Level: " << klein->getLevel() << std::endl;
        std::cout << "   Health: " << klein->getCurrentHealth() << "/" << klein->getMaxHealth() << std::endl;
        std::cout << "   Cor: " << klein->getCor() << std::endl;
        std::cout << std::endl;
        
        // ========================================
        // WORLD SYSTEM DEMONSTRATION
        // ========================================
        std::cout << "🌍 WORLD SYSTEM" << std::endl;
        std::cout << "===============" << std::endl;
        
        // Start the world
        std::cout << "Starting SAO world..." << std::endl;
        framework->startWorld();
        std::cout << "✅ World started successfully!" << std::endl;
        
        auto worldManager = framework->getWorldManager();
        auto worldSystem = worldManager->getWorldSystem();
        
        // Set initial locations
        SAO::World::Location townOfBeginnings(1, 0.0f, 0.0f, 0.0f);
        SAO::World::Location forestArea(1, 100.0f, 50.0f, 0.0f);
        SAO::World::Location floor2Location(2, 0.0f, 0.0f, 0.0f);
        
        worldSystem->setPlayerLocation(kirito, townOfBeginnings);
        worldSystem->setPlayerLocation(asuna, townOfBeginnings);
        worldSystem->setPlayerLocation(klein, townOfBeginnings);
        
        std::cout << "All players start in Town of Beginnings (Floor 1)" << std::endl;
        std::cout << std::endl;
        
        // Demonstrate teleportation
        std::cout << "Teleporting Kirito to Forest Area..." << std::endl;
        if (worldSystem->teleportPlayer(kirito, forestArea)) {
            std::cout << "✅ Kirito teleported to Forest Area!" << std::endl;
        }
        
        std::cout << "Teleporting Asuna to Floor 2..." << std::endl;
        if (worldSystem->teleportPlayer(asuna, floor2Location)) {
            std::cout << "✅ Asuna teleported to Floor 2!" << std::endl;
        }
        
        std::cout << std::endl;
        
        // ========================================
        // COMBAT SYSTEM DEMONSTRATION
        // ========================================
        std::cout << "⚔️  COMBAT SYSTEM" << std::endl;
        std::cout << "================" << std::endl;
        
        // Create some enemies
        std::cout << "Creating enemies..." << std::endl;
        auto goblin = framework->createPlayer("Goblin");
        auto wolf = framework->createPlayer("Dire Wolf");
        auto skeleton = framework->createPlayer("Skeleton Warrior");
        
        // Set enemy stats
        SAO::Core::PlayerStats goblinStats(8, 10, 12, 6, 4);
        SAO::Core::PlayerStats wolfStats(12, 14, 16, 8, 6);
        SAO::Core::PlayerStats skeletonStats(10, 12, 8, 10, 8);
        
        goblin->setBaseStats(goblinStats);
        wolf->setBaseStats(wolfStats);
        skeleton->setBaseStats(skeletonStats);
        
        std::cout << "✅ Enemies created!" << std::endl;
        std::cout << std::endl;
        
        // Combat 1: Kirito vs Goblin
        std::cout << "Combat 1: Kirito vs Goblin" << std::endl;
        std::cout << "-------------------------" << std::endl;
        
        framework->startCombat(kirito, goblin);
        
        auto combatManager = framework->getCombatManager();
        
        // Create and register sword skills
        auto horizontal = std::make_shared<SAO::Combat::BasicSwordSkill>(
            1, "Horizontal", SAO::Combat::BasicSwordSkill::BasicType::Horizontal
        );
        auto vertical = std::make_shared<SAO::Combat::BasicSwordSkill>(
            2, "Vertical", SAO::Combat::BasicSwordSkill::BasicType::Vertical
        );
        auto diagonal = std::make_shared<SAO::Combat::BasicSwordSkill>(
            3, "Diagonal", SAO::Combat::BasicSwordSkill::BasicType::Diagonal
        );
        
        combatManager->registerSwordSkill(horizontal);
        combatManager->registerSwordSkill(vertical);
        combatManager->registerSwordSkill(diagonal);
        
        // Kirito uses Horizontal
        if (horizontal->canUse()) {
            std::cout << "Kirito uses " << horizontal->getName() << "!" << std::endl;
            auto result = horizontal->execute(kirito, goblin);
            
            if (result.hit) {
                std::cout << "   Hit! Dealt " << result.damage << " damage!" << std::endl;
                if (result.critical) {
                    std::cout << "   Critical hit!" << std::endl;
                }
            } else {
                std::cout << "   Miss!" << std::endl;
            }
        }
        
        // Kirito uses Vertical
        if (vertical->canUse()) {
            std::cout << "Kirito uses " << vertical->getName() << "!" << std::endl;
            auto result = vertical->execute(kirito, goblin);
            
            if (result.hit) {
                std::cout << "   Hit! Dealt " << result.damage << " damage!" << std::endl;
                if (result.critical) {
                    std::cout << "   Critical hit!" << std::endl;
                }
            } else {
                std::cout << "   Miss!" << std::endl;
            }
        }
        
        std::cout << "Combat Results:" << std::endl;
        std::cout << "   Kirito HP: " << kirito->getCurrentHealth() << "/" << kirito->getMaxHealth() << std::endl;
        std::cout << "   Goblin HP: " << goblin->getCurrentHealth() << "/" << goblin->getMaxHealth() << std::endl;
        
        framework->endCombat();
        std::cout << std::endl;
        
        // Combat 2: Asuna vs Wolf
        std::cout << "Combat 2: Asuna vs Dire Wolf" << std::endl;
        std::cout << "----------------------------" << std::endl;
        
        framework->startCombat(asuna, wolf);
        
        // Create rapier-specific skills for Asuna
        auto thrust = std::make_shared<SAO::Combat::BasicSwordSkill>(
            4, "Thrust", SAO::Combat::BasicSwordSkill::BasicType::Thrust
        );
        auto guard = std::make_shared<SAO::Combat::BasicSwordSkill>(
            5, "Guard", SAO::Combat::BasicSwordSkill::BasicType::Guard
        );
        
        combatManager->registerSwordSkill(thrust);
        combatManager->registerSwordSkill(guard);
        
        // Asuna uses Thrust
        if (thrust->canUse()) {
            std::cout << "Asuna uses " << thrust->getName() << "!" << std::endl;
            auto result = thrust->execute(asuna, wolf);
            
            if (result.hit) {
                std::cout << "   Hit! Dealt " << result.damage << " damage!" << std::endl;
                if (result.critical) {
                    std::cout << "   Critical hit!" << std::endl;
                }
            } else {
                std::cout << "   Miss!" << std::endl;
            }
        }
        
        // Wolf attacks Asuna
        auto wolfAttack = std::make_shared<SAO::Combat::BasicSwordSkill>(
            6, "Bite", SAO::Combat::BasicSwordSkill::BasicType::Horizontal
        );
        combatManager->registerSwordSkill(wolfAttack);
        
        if (wolfAttack->canUse()) {
            std::cout << "Dire Wolf uses " << wolfAttack->getName() << "!" << std::endl;
            auto result = wolfAttack->execute(wolf, asuna);
            
            if (result.hit) {
                std::cout << "   Hit! Dealt " << result.damage << " damage!" << std::endl;
            } else {
                std::cout << "   Miss!" << std::endl;
            }
        }
        
        std::cout << "Combat Results:" << std::endl;
        std::cout << "   Asuna HP: " << asuna->getCurrentHealth() << "/" << asuna->getMaxHealth() << std::endl;
        std::cout << "   Dire Wolf HP: " << wolf->getCurrentHealth() << "/" << wolf->getMaxHealth() << std::endl;
        
        framework->endCombat();
        std::cout << std::endl;
        
        // ========================================
        // CHARACTER PROGRESSION DEMONSTRATION
        // ========================================
        std::cout << "📈 CHARACTER PROGRESSION" << std::endl;
        std::cout << "========================" << std::endl;
        
        // Add experience to all characters
        std::cout << "Adding experience to characters..." << std::endl;
        
        uint32_t kiritoExp = 800;
        uint32_t asunaExp = 600;
        uint32_t kleinExp = 400;
        
        kirito->addExperience(kiritoExp);
        asuna->addExperience(asunaExp);
        klein->addExperience(kleinExp);
        
        std::cout << "Kirito gained " << kiritoExp << " experience!" << std::endl;
        std::cout << "   Level: " << kirito->getLevel() << std::endl;
        std::cout << "   Experience: " << kirito->getExperience() << std::endl;
        std::cout << "   Skill points: " << kirito->getSkillPoints() << std::endl;
        
        std::cout << "Asuna gained " << asunaExp << " experience!" << std::endl;
        std::cout << "   Level: " << asuna->getLevel() << std::endl;
        std::cout << "   Experience: " << asuna->getExperience() << std::endl;
        std::cout << "   Skill points: " << asuna->getSkillPoints() << std::endl;
        
        std::cout << "Klein gained " << kleinExp << " experience!" << std::endl;
        std::cout << "   Level: " << klein->getLevel() << std::endl;
        std::cout << "   Experience: " << klein->getExperience() << std::endl;
        std::cout << "   Skill points: " << klein->getSkillPoints() << std::endl;
        
        std::cout << std::endl;
        
        // ========================================
        // EQUIPMENT AND INVENTORY DEMONSTRATION
        // ========================================
        std::cout << "🛡️  EQUIPMENT & INVENTORY" << std::endl;
        std::cout << "=========================" << std::endl;
        
        // Create some equipment
        std::cout << "Creating equipment..." << std::endl;
        
        // Kirito's dual swords
        auto kiritoSword1 = std::make_shared<SAO::Core::Weapon>(
            1, "Elucidator", SAO::Core::ItemRarity::Unique, 
            SAO::Core::Weapon::WeaponType::OneHandedSword
        );
        kiritoSword1->setDamage(120);
        kiritoSword1->setAttackSpeed(1.2f);
        kiritoSword1->setCriticalRate(15);
        
        auto kiritoSword2 = std::make_shared<SAO::Core::Weapon>(
            2, "Dark Repulser", SAO::Core::ItemRarity::Unique, 
            SAO::Core::Weapon::WeaponType::OneHandedSword
        );
        kiritoSword2->setDamage(110);
        kiritoSword2->setAttackSpeed(1.3f);
        kiritoSword2->setCriticalRate(12);
        
        // Asuna's rapier
        auto asunaRapier = std::make_shared<SAO::Core::Weapon>(
            3, "Wind Fleuret", SAO::Core::ItemRarity::Rare, 
            SAO::Core::Weapon::WeaponType::Rapier
        );
        asunaRapier->setDamage(95);
        asunaRapier->setAttackSpeed(1.8f);
        asunaRapier->setCriticalRate(20);
        
        // Add equipment to inventory
        kirito->addItemToInventory(kiritoSword1);
        kirito->addItemToInventory(kiritoSword2);
        asuna->addItemToInventory(asunaRapier);
        
        std::cout << "✅ Equipment created and added to inventory!" << std::endl;
        std::cout << "   Kirito inventory size: " << kirito->getInventorySize() << std::endl;
        std::cout << "   Asuna inventory size: " << asuna->getInventorySize() << std::endl;
        
        // Equip items
        std::cout << "Equipping items..." << std::endl;
        kirito->equipItem(kiritoSword1);
        asuna->equipItem(asunaRapier);
        
        std::cout << "✅ Items equipped!" << std::endl;
        std::cout << "   Kirito equipped: " << kiritoSword1->getName() << std::endl;
        std::cout << "   Asuna equipped: " << asunaRapier->getName() << std::endl;
        
        // Recalculate stats with equipment
        kirito->recalculateStats();
        asuna->recalculateStats();
        
        std::cout << "Updated stats with equipment:" << std::endl;
        const auto& kiritoTotalStats = kirito->getTotalStats();
        const auto& asunaTotalStats = asuna->getTotalStats();
        
        std::cout << "   Kirito - STR:" << kiritoTotalStats.strength 
                  << " DEX:" << kiritoTotalStats.dexterity 
                  << " AGI:" << kiritoTotalStats.agility 
                  << " VIT:" << kiritoTotalStats.vitality 
                  << " INT:" << kiritoTotalStats.intelligence << std::endl;
        
        std::cout << "   Asuna - STR:" << asunaTotalStats.strength 
                  << " DEX:" << asunaTotalStats.dexterity 
                  << " AGI:" << asunaTotalStats.agility 
                  << " VIT:" << asunaTotalStats.vitality 
                  << " INT:" << asunaTotalStats.intelligence << std::endl;
        
        std::cout << std::endl;
        
        // ========================================
        // CURRENCY AND ECONOMY DEMONSTRATION
        // ========================================
        std::cout << "💰 CURRENCY & ECONOMY" << std::endl;
        std::cout << "====================" << std::endl;
        
        // Add Cor to characters
        std::cout << "Adding Cor to characters..." << std::endl;
        
        kirito->addCor(2500);
        asuna->addCor(1800);
        klein->addCor(1200);
        
        std::cout << "Kirito Cor: " << kirito->getCor() << std::endl;
        std::cout << "Asuna Cor: " << asuna->getCor() << std::endl;
        std::cout << "Klein Cor: " << klein->getCor() << std::endl;
        
        // Demonstrate spending
        std::cout << "Kirito spends 500 Cor on repairs..." << std::endl;
        if (kirito->spendCor(500)) {
            std::cout << "✅ Repair successful! Remaining Cor: " << kirito->getCor() << std::endl;
        }
        
        std::cout << "Asuna spends 300 Cor on teleportation..." << std::endl;
        if (asuna->spendCor(300)) {
            std::cout << "✅ Teleportation successful! Remaining Cor: " << asuna->getCor() << std::endl;
        }
        
        std::cout << std::endl;
        
        // ========================================
        // FINAL STATUS AND CLEANUP
        // ========================================
        std::cout << "📊 FINAL STATUS" << std::endl;
        std::cout << "===============" << std::endl;
        
        std::cout << "Character Status:" << std::endl;
        std::cout << "   Kirito - Level " << kirito->getLevel() 
                  << ", HP " << kirito->getCurrentHealth() << "/" << kirito->getMaxHealth()
                  << ", Cor " << kirito->getCor() << std::endl;
        
        std::cout << "   Asuna - Level " << asuna->getLevel() 
                  << ", HP " << asuna->getCurrentHealth() << "/" << asuna->getMaxHealth()
                  << ", Cor " << asuna->getCor() << std::endl;
        
        std::cout << "   Klein - Level " << klein->getLevel() 
                  << ", HP " << klein->getCurrentHealth() << "/" << klein->getMaxHealth()
                  << ", Cor " << klein->getCor() << std::endl;
        
        std::cout << std::endl;
        std::cout << "System Status:" << std::endl;
        std::cout << "   World running: " << (framework->isWorldRunning() ? "Yes" : "No") << std::endl;
        std::cout << "   Combat active: " << (framework->isCombatActive() ? "Yes" : "No") << std::endl;
        std::cout << "   Framework initialized: " << (framework->isInitialized() ? "Yes" : "No") << std::endl;
        
        std::cout << std::endl;
        
        // Save all characters
        std::cout << "💾 Saving characters..." << std::endl;
        framework->savePlayer(kirito);
        framework->savePlayer(asuna);
        framework->savePlayer(klein);
        std::cout << "✅ All characters saved successfully!" << std::endl;
        
        std::cout << std::endl;
        
        // Shutdown
        std::cout << "🔄 Shutting down SAO Framework..." << std::endl;
        framework->stopWorld();
        frameworkManager.shutdownFramework();
        std::cout << "✅ SAO Framework shutdown complete!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "❌ Unknown error occurred!" << std::endl;
        return 1;
    }
    
    std::cout << std::endl;
    std::cout << "🎉 SAO Framework Basic Example completed successfully!" << std::endl;
    std::cout << "This example demonstrated:" << std::endl;
    std::cout << "   ✅ Character creation and customization" << std::endl;
    std::cout << "   ✅ World navigation and teleportation" << std::endl;
    std::cout << "   ✅ Combat system with sword skills" << std::endl;
    std::cout << "   ✅ Character progression and leveling" << std::endl;
    std::cout << "   ✅ Equipment and inventory management" << std::endl;
    std::cout << "   ✅ Currency and economy system" << std::endl;
    std::cout << "   ✅ Data persistence and saving" << std::endl;
    std::cout << std::endl;
    std::cout << "🗡️  Ready to create your own Sword Art Online experience!" << std::endl;
    
    return 0;
} 