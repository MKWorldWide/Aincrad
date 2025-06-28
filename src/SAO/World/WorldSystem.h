#pragma once

#include "../Core/PlayerSystem.h"
#include <vector>
#include <map>
#include <memory>
#include <string>

namespace SAO {
namespace World {

/**
 * @brief SAO Floor Information
 * 
 * Contains information about a specific floor in Aincrad,
 * including its layout, monsters, NPCs, and special features.
 */
struct FloorInfo {
    uint32_t floorNumber;           ///< Floor number (1-100)
    std::string name;               ///< Floor name
    std::string description;        ///< Floor description
    std::string theme;              ///< Visual theme (forest, desert, etc.)
    uint32_t minLevel;              ///< Minimum level required
    uint32_t maxLevel;              ///< Maximum level for optimal experience
    bool isSafeZone;                ///< Whether this floor has safe zones
    bool hasBoss;                   ///< Whether this floor has a boss
    std::string bossName;           ///< Name of the floor boss
    uint32_t bossLevel;             ///< Level of the floor boss
    
    FloorInfo() : floorNumber(0), minLevel(1), maxLevel(100), 
                  isSafeZone(false), hasBoss(false), bossLevel(0) {}
};

/**
 * @brief SAO Location Coordinates
 * 
 * Represents a position within the SAO world.
 */
struct Location {
    uint32_t floor;                 ///< Floor number
    float x;                        ///< X coordinate
    float y;                        ///< Y coordinate
    float z;                        ///< Z coordinate
    
    Location() : floor(1), x(0.0f), y(0.0f), z(0.0f) {}
    Location(uint32_t f, float xPos, float yPos, float zPos) 
        : floor(f), x(xPos), y(yPos), z(zPos) {}
};

/**
 * @brief SAO Teleport Gate
 * 
 * Represents a teleportation point in the SAO world.
 */
class TeleportGate {
public:
    TeleportGate(uint32_t id, const std::string& name, const Location& location);
    
    uint32_t getId() const { return id_; }
    const std::string& getName() const { return name_; }
    const Location& getLocation() const { return location_; }
    
    void setDestination(const Location& dest) { destination_ = dest; }
    const Location& getDestination() const { return destination_; }
    
    bool isActive() const { return active_; }
    void setActive(bool active) { active_ = active; }
    
    uint32_t getCost() const { return cost_; }
    void setCost(uint32_t cost) { cost_ = cost; }
    
private:
    uint32_t id_;
    std::string name_;
    Location location_;
    Location destination_;
    bool active_;
    uint32_t cost_;
};

/**
 * @brief SAO Safe Zone
 * 
 * Represents a safe area where combat is disabled.
 */
class SafeZone {
public:
    SafeZone(uint32_t id, const std::string& name, const Location& center, float radius);
    
    uint32_t getId() const { return id_; }
    const std::string& getName() const { return name_; }
    const Location& getCenter() const { return center_; }
    float getRadius() const { return radius_; }
    
    bool isPlayerInZone(const Location& playerLocation) const;
    
private:
    uint32_t id_;
    std::string name_;
    Location center_;
    float radius_;
};

/**
 * @brief SAO Monster Spawn Point
 * 
 * Defines where monsters can spawn in the world.
 */
class MonsterSpawnPoint {
public:
    MonsterSpawnPoint(uint32_t id, const Location& location, uint32_t monsterType);
    
    uint32_t getId() const { return id_; }
    const Location& getLocation() const { return location_; }
    uint32_t getMonsterType() const { return monsterType_; }
    
    void setSpawnRate(float rate) { spawnRate_ = rate; }
    float getSpawnRate() const { return spawnRate_; }
    
    void setMaxMonsters(uint32_t max) { maxMonsters_ = max; }
    uint32_t getMaxMonsters() const { return maxMonsters_; }
    
private:
    uint32_t id_;
    Location location_;
    uint32_t monsterType_;
    float spawnRate_;
    uint32_t maxMonsters_;
};

/**
 * @brief SAO Floor Manager
 * 
 * Manages a single floor of Aincrad including its layout,
 * teleport gates, safe zones, and monster spawns.
 */
class FloorManager {
public:
    FloorManager(uint32_t floorNumber);
    ~FloorManager() = default;
    
    // Floor information
    const FloorInfo& getFloorInfo() const { return floorInfo_; }
    void setFloorInfo(const FloorInfo& info) { floorInfo_ = info; }
    
    // Teleport gates
    void addTeleportGate(std::shared_ptr<TeleportGate> gate);
    std::shared_ptr<TeleportGate> getTeleportGate(uint32_t id) const;
    std::vector<std::shared_ptr<TeleportGate>> getTeleportGates() const;
    
    // Safe zones
    void addSafeZone(std::shared_ptr<SafeZone> zone);
    std::shared_ptr<SafeZone> getSafeZone(uint32_t id) const;
    std::vector<std::shared_ptr<SafeZone>> getSafeZones() const;
    bool isLocationSafe(const Location& location) const;
    
    // Monster spawns
    void addMonsterSpawn(std::shared_ptr<MonsterSpawnPoint> spawn);
    std::vector<std::shared_ptr<MonsterSpawnPoint>> getMonsterSpawns() const;
    
    // Floor utilities
    bool canPlayerAccess(std::shared_ptr<Core::PlayerCharacter> player) const;
    Location getSpawnLocation() const;
    void setSpawnLocation(const Location& location) { spawnLocation_ = location; }
    
private:
    FloorInfo floorInfo_;
    std::map<uint32_t, std::shared_ptr<TeleportGate>> teleportGates_;
    std::map<uint32_t, std::shared_ptr<SafeZone>> safeZones_;
    std::vector<std::shared_ptr<MonsterSpawnPoint>> monsterSpawns_;
    Location spawnLocation_;
};

/**
 * @brief SAO World System
 * 
 * Manages the entire SAO world including all floors,
 * teleportation, and world-wide mechanics.
 */
class WorldSystem {
public:
    WorldSystem();
    ~WorldSystem() = default;
    
    // Floor management
    void initializeFloors();
    std::shared_ptr<FloorManager> getFloor(uint32_t floorNumber) const;
    std::vector<std::shared_ptr<FloorManager>> getAllFloors() const;
    bool floorExists(uint32_t floorNumber) const;
    
    // Player location management
    void setPlayerLocation(std::shared_ptr<Core::PlayerCharacter> player, const Location& location);
    Location getPlayerLocation(std::shared_ptr<Core::PlayerCharacter> player) const;
    bool isPlayerInSafeZone(std::shared_ptr<Core::PlayerCharacter> player) const;
    
    // Teleportation
    bool teleportPlayer(std::shared_ptr<Core::PlayerCharacter> player, const Location& destination);
    bool useTeleportGate(std::shared_ptr<Core::PlayerCharacter> player, uint32_t gateId);
    std::vector<std::shared_ptr<TeleportGate>> getAvailableGates(std::shared_ptr<Core::PlayerCharacter> player) const;
    
    // World events
    void updateWorld();
    void spawnMonsters();
    void processWorldEvents();
    
    // Weather system
    void setWeather(uint32_t floor, const std::string& weather);
    std::string getWeather(uint32_t floor) const;
    void updateWeather();
    
    // Time system
    void setTimeOfDay(uint32_t hour, uint32_t minute);
    uint32_t getHour() const { return currentHour_; }
    uint32_t getMinute() const { return currentMinute_; }
    std::string getTimeString() const;
    
private:
    std::map<uint32_t, std::shared_ptr<FloorManager>> floors_;
    std::map<std::shared_ptr<Core::PlayerCharacter>, Location> playerLocations_;
    std::map<uint32_t, std::string> weatherConditions_;
    uint32_t currentHour_;
    uint32_t currentMinute_;
    
    // Private helper methods
    void initializeFloor1();
    void initializeFloor2();
    void initializeFloor3();
    void initializeFloor4();
    void initializeFloor5();
    float calculateDistance(const Location& loc1, const Location& loc2) const;
};

/**
 * @brief SAO World Manager
 * 
 * Global manager for all world-related systems and data.
 * Provides access to world configurations and global world state.
 */
class WorldManager {
public:
    static WorldManager& getInstance();
    
    // World system access
    std::shared_ptr<WorldSystem> getWorldSystem() const { return worldSystem_; }
    
    // World configuration
    void setWorldScale(float scale) { worldScale_ = scale; }
    float getWorldScale() const { return worldScale_; }
    
    void setDayNightCycle(bool enabled) { dayNightCycle_ = enabled; }
    bool isDayNightCycleEnabled() const { return dayNightCycle_; }
    
    void setWeatherEnabled(bool enabled) { weatherEnabled_ = enabled; }
    bool isWeatherEnabled() const { return weatherEnabled_; }
    
    // Global world state
    uint32_t getCurrentFloor() const { return currentFloor_; }
    void setCurrentFloor(uint32_t floor) { currentFloor_ = floor; }
    
    std::string getWorldStatus() const;
    void saveWorldState();
    void loadWorldState();
    
private:
    WorldManager();
    ~WorldManager() = default;
    WorldManager(const WorldManager&) = delete;
    WorldManager& operator=(const WorldManager&) = delete;
    
    std::shared_ptr<WorldSystem> worldSystem_;
    float worldScale_;
    bool dayNightCycle_;
    bool weatherEnabled_;
    uint32_t currentFloor_;
};

} // namespace World
} // namespace SAO 