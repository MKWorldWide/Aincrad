#include "SAOFramework.h"
#include <iostream>
#include <memory>
#include <string>

// Utility helpers stay in an anonymous namespace to avoid polluting the global scope.
namespace {
/**
 * @brief Print a concise summary of a player's core stats.
 *
 * This reduces repetition and centralizes display formatting.
 */
void printCharacterSummary(const std::shared_ptr<SAO::Player>& player) {
    std::cout << "✅ Created character: " << player->getName() << std::endl;
    std::cout << "   Level: " << player->getLevel() << std::endl;
    std::cout << "   Health: " << player->getCurrentHealth() << "/" << player->getMaxHealth() << std::endl;
    std::cout << "   Cor: " << player->getCor() << std::endl;
    std::cout << std::endl;
}
} // namespace

/**
 * @brief SAO Framework Main Entry Point
 * 
 * This is the main entry point for the Sword Art Online framework.
 * It demonstrates basic usage of the framework including character creation,
 * combat, and world management.
 */
int main(int argc, char* argv[]) {
    // Use '\n' to avoid flushing on each line, improving startup performance.
    std::cout << "🗡️  Sword Art Online Framework v2.0.0\n"
              << "=====================================\n\n";
    
    try {
        // Initialize the SAO framework
        // Newline avoids an unnecessary flush here as well.
        std::cout << "Initializing SAO Framework...\n";
        auto& frameworkManager = SAO::SAOFrameworkManager::getInstance();
        
        if (!frameworkManager.initializeFramework()) {
            std::cerr << "Failed to initialize SAO Framework!" << std::endl;
            return 1;
        }
        
        auto framework = frameworkManager.getFramework();
        std::cout << "✅ SAO Framework initialized successfully!" << std::endl;
        std::cout << std::endl;
        
        // Display framework information
        std::cout << "Framework Version: " << framework->getFrameworkVersion() << std::endl;
        std::cout << "Framework Status: " << framework->getFrameworkStatus() << std::endl;
        std::cout << std::endl;
        
        // Create a player character (Kirito)
        std::cout << "Creating player character..." << std::endl;
        auto kirito = framework->createPlayer("Kirito");
        
        // Set up Kirito's appearance
        SAO::Core::CharacterAppearance kiritoAppearance;
        kiritoAppearance.hairStyle = 1;      // Spiky black hair
        kiritoAppearance.hairColor = 0x000000; // Black
        kiritoAppearance.eyeColor = 0x000000;  // Black eyes
        kiritoAppearance.skinTone = 1;       // Fair skin
        kiritoAppearance.bodyType = 0;       // Average build
        kiritoAppearance.height = 1.0f;      // Normal height
        kiritoAppearance.weight = 1.0f;      // Normal weight
        
        // Set up Kirito's stats (high STR and DEX for dual wielding)
        SAO::Core::PlayerStats kiritoStats(16, 15, 13, 10, 8);
        
        kirito->setAppearance(kiritoAppearance);
        kirito->setBaseStats(kiritoStats);
        
        printCharacterSummary(kirito);
        
        // Create another character (Asuna)
        std::cout << "Creating second character..." << std::endl;
        auto asuna = framework->createPlayer("Asuna");
        
        // Set up Asuna's appearance
        SAO::Core::CharacterAppearance asunaAppearance;
        asunaAppearance.hairStyle = 2;       // Long hair
        asunaAppearance.hairColor = 0xFFD700; // Golden blonde
        asunaAppearance.eyeColor = 0x8B4513;  // Brown eyes
        asunaAppearance.skinTone = 0;        // Fair skin
        asunaAppearance.bodyType = 1;        // Slender build
        asunaAppearance.height = 0.95f;      // Slightly shorter
        asunaAppearance.weight = 0.9f;       // Slightly lighter
        
        // Set up Asuna's stats (high DEX and AGI for rapier)
        SAO::Core::PlayerStats asunaStats(10, 18, 16, 8, 12);
        
        asuna->setAppearance(asunaAppearance);
        asuna->setBaseStats(asunaStats);
        
        printCharacterSummary(asuna);
        
        // Start the world
        std::cout << "Starting SAO world..." << std::endl;
        framework->startWorld();
        std::cout << "✅ World started successfully!" << std::endl;
        std::cout << std::endl;
        
        // Demonstrate combat system
        std::cout << "=== Combat Demonstration ===" << std::endl;
        
        // Start combat between Kirito and Asuna
        framework->startCombat(kirito, asuna);
        std::cout << "Combat started between " << kirito->getName() << " and " << asuna->getName() << std::endl;
        
        // Get combat manager
        auto combatManager = framework->getCombatManager();
        
        // Create a basic sword skill
        auto horizontal = std::make_shared<SAO::Combat::BasicSwordSkill>(
            1, "Horizontal", SAO::Combat::BasicSwordSkill::BasicType::Horizontal
        );
        
        // Register the skill
        combatManager->registerSwordSkill(horizontal);
        
        // Use the skill
        if (horizontal->canUse()) {
            std::cout << kirito->getName() << " uses " << horizontal->getName() << "!" << std::endl;
            auto result = horizontal->execute(kirito, asuna);
            
            if (result.hit) {
                std::cout << "   Hit! Dealt " << result.damage << " damage!" << std::endl;
                if (result.critical) {
                    std::cout << "   Critical hit!" << std::endl;
                }
            } else {
                std::cout << "   Miss!" << std::endl;
            }
            
            if (result.blocked) {
                std::cout << "   Attack was blocked!" << std::endl;
            }
            if (result.parried) {
                std::cout << "   Attack was parried!" << std::endl;
            }
            if (result.dodged) {
                std::cout << "   Attack was dodged!" << std::endl;
            }
        }
        
        std::cout << std::endl;
        std::cout << "Combat Results:" << std::endl;
        std::cout << "   " << kirito->getName() << " HP: " << kirito->getCurrentHealth() << "/" << kirito->getMaxHealth() << std::endl;
        std::cout << "   " << asuna->getName() << " HP: " << asuna->getCurrentHealth() << "/" << asuna->getMaxHealth() << std::endl;
        std::cout << std::endl;
        
        // End combat
        framework->endCombat();
        std::cout << "Combat ended." << std::endl;
        std::cout << std::endl;
        
        // Demonstrate world system
        std::cout << "=== World System Demonstration ===" << std::endl;
        
        auto worldManager = framework->getWorldManager();
        auto worldSystem = worldManager->getWorldSystem();
        
        // Set player locations
        SAO::World::Location kiritoLocation(1, 100.0f, 50.0f, 0.0f);
        SAO::World::Location asunaLocation(1, 150.0f, 75.0f, 0.0f);
        
        worldSystem->setPlayerLocation(kirito, kiritoLocation);
        worldSystem->setPlayerLocation(asuna, asunaLocation);
        
        std::cout << kirito->getName() << " is at Floor " << kiritoLocation.floor 
                  << " (" << kiritoLocation.x << ", " << kiritoLocation.y << ", " << kiritoLocation.z << ")" << std::endl;
        std::cout << asuna->getName() << " is at Floor " << asunaLocation.floor 
                  << " (" << asunaLocation.x << ", " << asunaLocation.y << ", " << asunaLocation.z << ")" << std::endl;
        
        // Check if players are in safe zones
        bool kiritoSafe = worldSystem->isPlayerInSafeZone(kirito);
        bool asunaSafe = worldSystem->isPlayerInSafeZone(asuna);
        
        std::cout << kirito->getName() << " in safe zone: " << (kiritoSafe ? "Yes" : "No") << std::endl;
        std::cout << asuna->getName() << " in safe zone: " << (asunaSafe ? "Yes" : "No") << std::endl;
        
        // Teleport Kirito to floor 2
        SAO::World::Location floor2Location(2, 0.0f, 0.0f, 0.0f);
        if (worldSystem->teleportPlayer(kirito, floor2Location)) {
            std::cout << kirito->getName() << " teleported to Floor 2!" << std::endl;
        }
        
        std::cout << std::endl;
        
        // Demonstrate character progression
        std::cout << "=== Character Progression ===" << std::endl;
        
        // Add experience to Kirito
        uint32_t expGained = 500;
        kirito->addExperience(expGained);
        std::cout << kirito->getName() << " gained " << expGained << " experience!" << std::endl;
        std::cout << "   Level: " << kirito->getLevel() << std::endl;
        std::cout << "   Experience: " << kirito->getExperience() << std::endl;
        std::cout << "   Experience to next level: " << kirito->getExperienceToNext() << std::endl;
        std::cout << "   Skill points: " << kirito->getSkillPoints() << std::endl;
        
        // Show updated stats
        const auto& totalStats = kirito->getTotalStats();
        std::cout << "   Total Stats - STR:" << totalStats.strength 
                  << " DEX:" << totalStats.dexterity 
                  << " AGI:" << totalStats.agility 
                  << " VIT:" << totalStats.vitality 
                  << " INT:" << totalStats.intelligence << std::endl;
        
        std::cout << std::endl;
        
        // Save characters
        std::cout << "Saving characters..." << std::endl;
        framework->savePlayer(kirito);
        framework->savePlayer(asuna);
        std::cout << "✅ Characters saved successfully!" << std::endl;
        std::cout << std::endl;
        
        // Display final status
        std::cout << "=== Final Status ===" << std::endl;
        std::cout << "World running: " << (framework->isWorldRunning() ? "Yes" : "No") << std::endl;
        std::cout << "Combat active: " << (framework->isCombatActive() ? "Yes" : "No") << std::endl;
        std::cout << "Framework initialized: " << (framework->isInitialized() ? "Yes" : "No") << std::endl;
        std::cout << std::endl;
        
        // Shutdown
        std::cout << "Shutting down SAO Framework..." << std::endl;
        framework->stopWorld();
        frameworkManager.shutdownFramework();
        std::cout << "✅ SAO Framework shutdown complete!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred!" << std::endl;
        return 1;
    }
    
    std::cout << std::endl;
    std::cout << "🗡️  Thank you for using the Sword Art Online Framework!" << std::endl;
    std::cout << "For more information, visit: https://github.com/yourusername/sao-framework" << std::endl;
    
    return 0;
}
