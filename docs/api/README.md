# 🗡️ SAO Framework API Documentation

Welcome to the comprehensive API documentation for the Sword Art Online Framework. This documentation covers all classes, methods, and systems available in the framework.

## 📚 Table of Contents

- [Core Systems](#core-systems)
- [Combat System](#combat-system)
- [World System](#world-system)
- [Framework API](#framework-api)
- [Examples](#examples)
- [Best Practices](#best-practices)

## 🎯 Core Systems

### Player System

The player system manages character creation, progression, and customization.

#### PlayerCharacter Class

```cpp
class PlayerCharacter {
public:
    // Constructors
    PlayerCharacter(const std::string& name);
    
    // Basic Information
    const std::string& getName() const;
    uint32_t getLevel() const;
    uint32_t getExperience() const;
    uint32_t getExperienceToNext() const;
    
    // Stats and Progression
    const PlayerStats& getBaseStats() const;
    const PlayerStats& getTotalStats() const;
    const CharacterAppearance& getAppearance() const;
    void setBaseStats(const PlayerStats& stats);
    void setAppearance(const CharacterAppearance& appearance);
    
    // Leveling and Experience
    void addExperience(uint32_t exp);
    void levelUp();
    uint32_t getSkillPoints() const;
    void spendSkillPoints(uint32_t points);
    
    // Health and Status
    uint32_t getCurrentHealth() const;
    uint32_t getMaxHealth() const;
    bool isAlive() const;
    void heal(uint32_t amount);
    void damage(uint32_t amount);
    void revive();
    
    // Equipment Management
    bool equipItem(std::shared_ptr<Equipment> item);
    std::shared_ptr<Equipment> unequipItem(EquipmentSlot slot);
    std::shared_ptr<Equipment> getEquippedItem(EquipmentSlot slot) const;
    bool isSlotOccupied(EquipmentSlot slot) const;
    
    // Inventory Management
    bool addItemToInventory(std::shared_ptr<Item> item);
    bool removeItemFromInventory(uint32_t itemId, uint32_t quantity = 1);
    std::vector<std::shared_ptr<Item>> getInventory() const;
    uint32_t getInventorySize() const;
    uint32_t getMaxInventorySize() const;
    
    // Currency
    uint32_t getCor() const;
    void addCor(uint32_t amount);
    bool spendCor(uint32_t amount);
    
    // Utility Methods
    void recalculateStats();
    void saveToDatabase();
    void loadFromDatabase();
};
```

#### PlayerStats Structure

```cpp
struct PlayerStats {
    uint32_t strength;      // Physical attack power and carrying capacity
    uint32_t dexterity;     // Attack speed, accuracy, and critical hit rate
    uint32_t agility;       // Movement speed, dodge rate, and reaction time
    uint32_t vitality;      // Health points and physical defense
    uint32_t intelligence;  // Magic power, skill effectiveness, and learning speed
    
    PlayerStats();
    PlayerStats(uint32_t str, uint32_t dex, uint32_t agi, uint32_t vit, uint32_t intel);
};
```

#### CharacterAppearance Structure

```cpp
struct CharacterAppearance {
    uint32_t faceType;      // Face shape and features
    uint32_t hairStyle;     // Hair style and length
    uint32_t hairColor;     // Hair color (RGB encoded)
    uint32_t eyeColor;      // Eye color (RGB encoded)
    uint32_t skinTone;      // Skin tone and complexion
    uint32_t bodyType;      // Body build and proportions
    float height;           // Character height multiplier
    float weight;           // Character weight multiplier
    
    CharacterAppearance();
};
```

#### CharacterCreationSystem Class

```cpp
class CharacterCreationSystem {
public:
    struct CreationOptions {
        std::string name;
        CharacterAppearance appearance;
        PlayerStats startingStats;
        uint32_t startingCor;
        std::vector<std::shared_ptr<Item>> startingItems;
    };
    
    static std::shared_ptr<PlayerCharacter> createCharacter(const CreationOptions& options);
    static bool validateCharacterName(const std::string& name);
    static bool validateStartingStats(const PlayerStats& stats);
    static std::vector<std::string> getAvailableNames();
};
```

### Equipment System

The equipment system manages weapons, armor, and items.

#### Item Base Class

```cpp
class Item {
public:
    Item(uint32_t id, const std::string& name, ItemRarity rarity);
    virtual ~Item() = default;
    
    // Getters
    uint32_t getId() const;
    const std::string& getName() const;
    ItemRarity getRarity() const;
    uint32_t getDurability() const;
    uint32_t getMaxDurability() const;
    uint32_t getValue() const;
    
    // Setters
    void setDurability(uint32_t durability);
    void setValue(uint32_t value);
    
    // Virtual Methods
    virtual bool isEquippable() const = 0;
    virtual bool isConsumable() const = 0;
    virtual bool isStackable() const = 0;
    virtual uint32_t getMaxStack() const;
    
    // Durability Management
    bool isBroken() const;
    void repair();
    void damage(uint32_t amount);
};
```

#### Equipment Class

```cpp
class Equipment : public Item {
public:
    Equipment(uint32_t id, const std::string& name, ItemRarity rarity, EquipmentSlot slot);
    
    bool isEquippable() const override;
    bool isConsumable() const override;
    bool isStackable() const override;
    
    EquipmentSlot getSlot() const;
    const PlayerStats& getStatBonuses() const;
    void setStatBonuses(const PlayerStats& stats);
};
```

#### Weapon Class

```cpp
class Weapon : public Equipment {
public:
    enum class WeaponType {
        OneHandedSword,
        TwoHandedSword,
        Rapier,
        Dagger,
        Spear,
        Axe,
        Mace,
        Bow,
        Staff,
        Shield
    };
    
    Weapon(uint32_t id, const std::string& name, ItemRarity rarity, WeaponType type);
    
    WeaponType getWeaponType() const;
    uint32_t getDamage() const;
    float getAttackSpeed() const;
    uint32_t getRange() const;
    uint32_t getCriticalRate() const;
    
    void setDamage(uint32_t damage);
    void setAttackSpeed(float speed);
    void setRange(uint32_t range);
    void setCriticalRate(uint32_t rate);
};
```

#### Armor Class

```cpp
class Armor : public Equipment {
public:
    enum class ArmorType {
        Light,      // Cloth, leather - high mobility, low defense
        Medium,     // Chain, scale - balanced mobility and defense
        Heavy       // Plate, full armor - low mobility, high defense
    };
    
    Armor(uint32_t id, const std::string& name, ItemRarity rarity, ArmorType type);
    
    ArmorType getArmorType() const;
    uint32_t getPhysicalDefense() const;
    uint32_t getMagicalDefense() const;
    float getWeight() const;
    
    void setPhysicalDefense(uint32_t defense);
    void setMagicalDefense(uint32_t defense);
    void setWeight(float weight);
};
```

## ⚔️ Combat System

The combat system manages sword skills, combat mechanics, and battle resolution.

### CombatSystem Class

```cpp
class CombatSystem {
public:
    CombatSystem();
    ~CombatSystem() = default;
    
    // Combat Initialization
    void initializeCombat(std::shared_ptr<PlayerCharacter> player1,
                         std::shared_ptr<PlayerCharacter> player2);
    void initializeCombat(std::vector<std::shared_ptr<PlayerCharacter>> players);
    
    // Combat Execution
    CombatResult performAttack(std::shared_ptr<PlayerCharacter> attacker,
                              std::shared_ptr<PlayerCharacter> target);
    CombatResult useSwordSkill(std::shared_ptr<PlayerCharacter> user,
                              std::shared_ptr<PlayerCharacter> target,
                              std::shared_ptr<SwordSkill> skill);
    
    // Combat State
    bool isCombatActive() const;
    std::vector<std::shared_ptr<PlayerCharacter>> getCombatants() const;
    std::shared_ptr<PlayerCharacter> getCurrentTurn() const;
    
    // Combat Flow
    void startCombat();
    void endCombat();
    void nextTurn();
    
    // Combat Mechanics
    bool canAttack(std::shared_ptr<PlayerCharacter> attacker,
                   std::shared_ptr<PlayerCharacter> target) const;
    uint32_t calculateDamage(std::shared_ptr<PlayerCharacter> attacker,
                            std::shared_ptr<PlayerCharacter> target,
                            DamageType damageType = DamageType::Physical);
    bool checkHit(std::shared_ptr<PlayerCharacter> attacker,
                  std::shared_ptr<PlayerCharacter> target) const;
    bool checkCritical(std::shared_ptr<PlayerCharacter> attacker) const;
    bool checkBlock(std::shared_ptr<PlayerCharacter> target) const;
    bool checkParry(std::shared_ptr<PlayerCharacter> target) const;
    bool checkDodge(std::shared_ptr<PlayerCharacter> target) const;
    
    // Skill Management
    void registerSwordSkill(std::shared_ptr<SwordSkill> skill);
    std::vector<std::shared_ptr<SwordSkill>> getAvailableSkills(std::shared_ptr<PlayerCharacter> user) const;
    
    // Combat Events
    using CombatEventHandler = std::function<void(const CombatResult&)>;
    void addCombatEventHandler(CombatEventHandler handler);
    void removeCombatEventHandler(CombatEventHandler handler);
};
```

### SwordSkill Base Class

```cpp
class SwordSkill {
public:
    enum class SkillType {
        Basic,          // Basic sword skills (Horizontal, Vertical, Diagonal)
        Advanced,       // Advanced sword skills (Linear, Arc, Spiral)
        Unique,         // Unique sword skills (Starburst Stream, Vorpal Strike)
        Ultimate        // Ultimate sword skills (The Eclipse, etc.)
    };
    
    SwordSkill(uint32_t id, const std::string& name, SkillType type, uint32_t cooldown);
    virtual ~SwordSkill() = default;
    
    // Getters
    uint32_t getId() const;
    const std::string& getName() const;
    SkillType getType() const;
    uint32_t getCooldown() const;
    uint32_t getLevel() const;
    uint32_t getMaxLevel() const;
    
    // Skill Progression
    void levelUp();
    bool canLevelUp() const;
    uint32_t getExperienceToNext() const;
    void addExperience(uint32_t exp);
    
    // Combat Usage
    bool canUse() const;
    void use();
    virtual CombatResult execute(std::shared_ptr<PlayerCharacter> user, 
                                std::shared_ptr<PlayerCharacter> target) = 0;
    
    // Cooldown Management
    bool isOnCooldown() const;
    uint32_t getRemainingCooldown() const;
    void resetCooldown();
};
```

### CombatResult Structure

```cpp
struct CombatResult {
    bool hit;                   // Whether the attack hit
    bool critical;              // Whether it was a critical hit
    uint32_t damage;            // Total damage dealt
    DamageType damageType;      // Type of damage dealt
    bool blocked;               // Whether the attack was blocked
    bool parried;               // Whether the attack was parried
    bool dodged;                // Whether the attack was dodged
    std::vector<std::string> effects; // Special effects applied
    
    CombatResult();
};
```

### Unique Sword Skills

#### StarburstStream Class

```cpp
class StarburstStream : public UniqueSwordSkill {
public:
    StarburstStream();
    
    CombatResult execute(std::shared_ptr<PlayerCharacter> user, 
                        std::shared_ptr<PlayerCharacter> target) override;
    
protected:
    uint32_t calculateDamage(std::shared_ptr<PlayerCharacter> user) override;
    std::vector<std::string> getSpecialEffects() override;
    
private:
    static const uint32_t COMBO_HITS = 16;
    static const uint32_t BASE_DAMAGE_PER_HIT = 50;
};
```

#### VorpalStrike Class

```cpp
class VorpalStrike : public UniqueSwordSkill {
public:
    VorpalStrike();
    
    CombatResult execute(std::shared_ptr<PlayerCharacter> user, 
                        std::shared_ptr<PlayerCharacter> target) override;
    
protected:
    uint32_t calculateDamage(std::shared_ptr<PlayerCharacter> user) override;
    std::vector<std::string> getSpecialEffects() override;
    
private:
    static const uint32_t BASE_DAMAGE = 200;
    static const uint32_t CRITICAL_BONUS = 100;
};
```

## 🌍 World System

The world system manages the SAO universe, including floors, teleportation, and world mechanics.

### WorldSystem Class

```cpp
class WorldSystem {
public:
    WorldSystem();
    ~WorldSystem() = default;
    
    // Floor Management
    void initializeFloors();
    std::shared_ptr<FloorManager> getFloor(uint32_t floorNumber) const;
    std::vector<std::shared_ptr<FloorManager>> getAllFloors() const;
    bool floorExists(uint32_t floorNumber) const;
    
    // Player Location Management
    void setPlayerLocation(std::shared_ptr<PlayerCharacter> player, const Location& location);
    Location getPlayerLocation(std::shared_ptr<PlayerCharacter> player) const;
    bool isPlayerInSafeZone(std::shared_ptr<PlayerCharacter> player) const;
    
    // Teleportation
    bool teleportPlayer(std::shared_ptr<PlayerCharacter> player, const Location& destination);
    bool useTeleportGate(std::shared_ptr<PlayerCharacter> player, uint32_t gateId);
    std::vector<std::shared_ptr<TeleportGate>> getAvailableGates(std::shared_ptr<PlayerCharacter> player) const;
    
    // World Events
    void updateWorld();
    void spawnMonsters();
    void processWorldEvents();
    
    // Weather System
    void setWeather(uint32_t floor, const std::string& weather);
    std::string getWeather(uint32_t floor) const;
    void updateWeather();
    
    // Time System
    void setTimeOfDay(uint32_t hour, uint32_t minute);
    uint32_t getHour() const;
    uint32_t getMinute() const;
    std::string getTimeString() const;
};
```

### Location Structure

```cpp
struct Location {
    uint32_t floor;                 // Floor number
    float x;                        // X coordinate
    float y;                        // Y coordinate
    float z;                        // Z coordinate
    
    Location();
    Location(uint32_t f, float xPos, float yPos, float zPos);
};
```

### FloorManager Class

```cpp
class FloorManager {
public:
    FloorManager(uint32_t floorNumber);
    ~FloorManager() = default;
    
    // Floor Information
    const FloorInfo& getFloorInfo() const;
    void setFloorInfo(const FloorInfo& info);
    
    // Teleport Gates
    void addTeleportGate(std::shared_ptr<TeleportGate> gate);
    std::shared_ptr<TeleportGate> getTeleportGate(uint32_t id) const;
    std::vector<std::shared_ptr<TeleportGate>> getTeleportGates() const;
    
    // Safe Zones
    void addSafeZone(std::shared_ptr<SafeZone> zone);
    std::shared_ptr<SafeZone> getSafeZone(uint32_t id) const;
    std::vector<std::shared_ptr<SafeZone>> getSafeZones() const;
    bool isLocationSafe(const Location& location) const;
    
    // Monster Spawns
    void addMonsterSpawn(std::shared_ptr<MonsterSpawnPoint> spawn);
    std::vector<std::shared_ptr<MonsterSpawnPoint>> getMonsterSpawns() const;
    
    // Floor Utilities
    bool canPlayerAccess(std::shared_ptr<PlayerCharacter> player) const;
    Location getSpawnLocation() const;
    void setSpawnLocation(const Location& location);
};
```

## 🏗️ Framework API

### SAOFramework Class

```cpp
class SAOFramework {
public:
    SAOFramework();
    ~SAOFramework();
    
    // Framework Initialization
    bool initialize();
    void shutdown();
    bool isInitialized() const;
    
    // System Access
    std::shared_ptr<CharacterCreationSystem> getCharacterCreation() const;
    std::shared_ptr<CombatManager> getCombatManager() const;
    std::shared_ptr<WorldManager> getWorldManager() const;
    
    // Player Management
    std::shared_ptr<PlayerCharacter> createPlayer(const std::string& name);
    void savePlayer(std::shared_ptr<PlayerCharacter> player);
    std::shared_ptr<PlayerCharacter> loadPlayer(const std::string& name);
    bool deletePlayer(const std::string& name);
    
    // World Management
    void startWorld();
    void stopWorld();
    void updateWorld();
    bool isWorldRunning() const;
    
    // Combat Management
    void startCombat(std::shared_ptr<PlayerCharacter> player1,
                     std::shared_ptr<PlayerCharacter> player2);
    void endCombat();
    bool isCombatActive() const;
    
    // Configuration
    void setConfiguration(const std::string& key, const std::string& value);
    std::string getConfiguration(const std::string& key) const;
    
    // Logging and Debugging
    void setLogLevel(int level);
    void logMessage(const std::string& message);
    void logError(const std::string& error);
    void logWarning(const std::string& warning);
    
    // Framework Status
    std::string getFrameworkVersion() const;
    std::string getFrameworkStatus() const;
};
```

### SAOFrameworkManager Class

```cpp
class SAOFrameworkManager {
public:
    static SAOFrameworkManager& getInstance();
    
    std::shared_ptr<SAOFramework> getFramework() const;
    bool initializeFramework();
    void shutdownFramework();
};
```

## 📝 Examples

### Basic Character Creation

```cpp
#include "SAO/SAOFramework.h"

int main() {
    // Initialize framework
    auto& frameworkManager = SAO::SAOFrameworkManager::getInstance();
    frameworkManager.initializeFramework();
    auto framework = frameworkManager.getFramework();
    
    // Create character
    auto player = framework->createPlayer("Kirito");
    
    // Set appearance
    SAO::Core::CharacterAppearance appearance;
    appearance.hairStyle = 1;
    appearance.hairColor = 0x000000; // Black
    player->setAppearance(appearance);
    
    // Set stats
    SAO::Core::PlayerStats stats(16, 15, 13, 10, 8);
    player->setBaseStats(stats);
    
    // Start world
    framework->startWorld();
    
    return 0;
}
```

### Combat Example

```cpp
#include "SAO/SAOFramework.h"

void combatExample() {
    auto framework = SAO::SAOFrameworkManager::getInstance().getFramework();
    
    // Create characters
    auto kirito = framework->createPlayer("Kirito");
    auto enemy = framework->createPlayer("Goblin");
    
    // Start combat
    framework->startCombat(kirito, enemy);
    
    // Get combat manager
    auto combatManager = framework->getCombatManager();
    
    // Create sword skill
    auto horizontal = std::make_shared<SAO::Combat::BasicSwordSkill>(
        1, "Horizontal", SAO::Combat::BasicSwordSkill::BasicType::Horizontal
    );
    
    // Register and use skill
    combatManager->registerSwordSkill(horizontal);
    auto result = horizontal->execute(kirito, enemy);
    
    if (result.hit) {
        std::cout << "Dealt " << result.damage << " damage!" << std::endl;
    }
    
    framework->endCombat();
}
```

### World Navigation Example

```cpp
#include "SAO/SAOFramework.h"

void worldExample() {
    auto framework = SAO::SAOFrameworkManager::getInstance().getFramework();
    auto worldSystem = framework->getWorldManager()->getWorldSystem();
    
    auto player = framework->createPlayer("Player");
    
    // Set location
    SAO::World::Location location(1, 100.0f, 50.0f, 0.0f);
    worldSystem->setPlayerLocation(player, location);
    
    // Teleport to different floor
    SAO::World::Location newLocation(2, 0.0f, 0.0f, 0.0f);
    worldSystem->teleportPlayer(player, newLocation);
    
    // Check if in safe zone
    bool isSafe = worldSystem->isPlayerInSafeZone(player);
    std::cout << "Player in safe zone: " << (isSafe ? "Yes" : "No") << std::endl;
}
```

## 🎯 Best Practices

### Performance Optimization

1. **Reuse Objects**: Create and reuse sword skills and equipment objects instead of creating new ones frequently.

2. **Batch Operations**: When possible, batch multiple operations together to reduce overhead.

3. **Memory Management**: Use smart pointers (`std::shared_ptr`) for automatic memory management.

4. **Efficient Iteration**: Use range-based for loops when iterating over collections.

### Error Handling

1. **Check Return Values**: Always check return values from framework methods.

2. **Exception Safety**: Wrap framework operations in try-catch blocks for robust error handling.

3. **Validation**: Validate input parameters before passing them to framework methods.

### Code Organization

1. **Separation of Concerns**: Keep different systems (combat, world, player) separate in your code.

2. **Resource Management**: Properly initialize and shutdown the framework in your application.

3. **Configuration**: Use the configuration system for customizable parameters.

### Multiplayer Considerations

1. **State Synchronization**: Ensure player states are properly synchronized in multiplayer scenarios.

2. **Network Latency**: Account for network latency when implementing real-time features.

3. **Anti-Cheat**: Implement server-side validation for all critical operations.

## 🔧 Configuration Options

The SAO Framework supports various configuration options:

```cpp
// VR Support
framework->setConfiguration("VR_ENABLED", "true");
framework->setConfiguration("VR_DEVICE", "oculus_quest");

// Multiplayer
framework->setConfiguration("MULTIPLAYER_ENABLED", "true");
framework->setConfiguration("SERVER_ADDRESS", "localhost:8080");

// Performance
framework->setConfiguration("MAX_PLAYERS", "1000");
framework->setConfiguration("TICK_RATE", "60");

// Debug
framework->setConfiguration("DEBUG_MODE", "true");
framework->setConfiguration("LOG_LEVEL", "info");
```

## 📞 Support

For questions, issues, or contributions:

- **GitHub Issues**: [Report bugs and request features](https://github.com/yourusername/sao-framework/issues)
- **Discussions**: [Join community discussions](https://github.com/yourusername/sao-framework/discussions)
- **Documentation**: [View additional documentation](https://github.com/yourusername/sao-framework/wiki)

---

**Ready to create your own Sword Art Online experience? Start building with the SAO Framework today!** 🗡️✨ 