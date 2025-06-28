#pragma once

#include "Core/PlayerSystem.h"
#include "Combat/CombatSystem.h"
#include "World/WorldSystem.h"
#include <memory>
#include <string>

namespace SAO {

/**
 * @brief SAO Framework Main Class
 * 
 * The main entry point for the Sword Art Online framework.
 * Manages all core systems and provides a unified interface
 * for creating SAO-inspired applications and games.
 */
class SAOFramework {
public:
    SAOFramework();
    ~SAOFramework();
    
    // Framework initialization
    bool initialize();
    void shutdown();
    bool isInitialized() const { return initialized_; }
    
    // System access
    std::shared_ptr<Core::CharacterCreationSystem> getCharacterCreation() const;
    std::shared_ptr<Combat::CombatManager> getCombatManager() const;
    std::shared_ptr<World::WorldManager> getWorldManager() const;
    
    // Player management
    std::shared_ptr<Core::PlayerCharacter> createPlayer(const std::string& name);
    void savePlayer(std::shared_ptr<Core::PlayerCharacter> player);
    std::shared_ptr<Core::PlayerCharacter> loadPlayer(const std::string& name);
    bool deletePlayer(const std::string& name);
    
    // World management
    void startWorld();
    void stopWorld();
    void updateWorld();
    bool isWorldRunning() const { return worldRunning_; }
    
    // Combat management
    void startCombat(std::shared_ptr<Core::PlayerCharacter> player1,
                     std::shared_ptr<Core::PlayerCharacter> player2);
    void endCombat();
    bool isCombatActive() const;
    
    // Configuration
    void setConfiguration(const std::string& key, const std::string& value);
    std::string getConfiguration(const std::string& key) const;
    
    // Logging and debugging
    void setLogLevel(int level);
    void logMessage(const std::string& message);
    void logError(const std::string& error);
    void logWarning(const std::string& warning);
    
    // Framework status
    std::string getFrameworkVersion() const;
    std::string getFrameworkStatus() const;
    
private:
    bool initialized_;
    bool worldRunning_;
    
    // Core systems
    std::shared_ptr<Core::CharacterCreationSystem> characterCreation_;
    std::shared_ptr<Combat::CombatManager> combatManager_;
    std::shared_ptr<World::WorldManager> worldManager_;
    
    // Configuration
    std::map<std::string, std::string> configuration_;
    
    // Private helper methods
    void initializeSystems();
    void shutdownSystems();
    void loadDefaultConfiguration();
};

/**
 * @brief SAO Framework Singleton
 * 
 * Global access point to the SAO framework instance.
 */
class SAOFrameworkManager {
public:
    static SAOFrameworkManager& getInstance();
    
    std::shared_ptr<SAOFramework> getFramework() const { return framework_; }
    bool initializeFramework();
    void shutdownFramework();
    
private:
    SAOFrameworkManager() = default;
    ~SAOFrameworkManager() = default;
    SAOFrameworkManager(const SAOFrameworkManager&) = delete;
    SAOFrameworkManager& operator=(const SAOFrameworkManager&) = delete;
    
    std::shared_ptr<SAOFramework> framework_;
};

} // namespace SAO 