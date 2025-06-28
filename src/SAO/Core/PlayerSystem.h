#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cstdint>
#include <algorithm>

namespace SAO {
namespace Core {

/**
 * @brief SAO Player Statistics
 * 
 * Represents the core statistics of a player character in the Sword Art Online universe.
 * These stats determine combat effectiveness, survivability, and character progression.
 */
struct PlayerStats {
    uint32_t strength;      ///< Physical attack power and carrying capacity
    uint32_t dexterity;     ///< Attack speed, accuracy, and critical hit rate
    uint32_t agility;       ///< Movement speed, dodge rate, and reaction time
    uint32_t vitality;      ///< Health points and physical defense
    uint32_t intelligence;  ///< Magic power, skill effectiveness, and learning speed
    
    PlayerStats() : strength(10), dexterity(10), agility(10), vitality(10), intelligence(10) {}
    PlayerStats(uint32_t str, uint32_t dex, uint32_t agi, uint32_t vit, uint32_t intel)
        : strength(str), dexterity(dex), agility(agi), vitality(vit), intelligence(intel) {}
};

/**
 * @brief SAO Character Appearance
 * 
 * Defines the visual appearance of a player character including
 * facial features, body type, and customization options.
 */
struct CharacterAppearance {
    uint32_t faceType;      ///< Face shape and features
    uint32_t hairStyle;     ///< Hair style and length
    uint32_t hairColor;     ///< Hair color (RGB encoded)
    uint32_t eyeColor;      ///< Eye color (RGB encoded)
    uint32_t skinTone;      ///< Skin tone and complexion
    uint32_t bodyType;      ///< Body build and proportions
    float height;           ///< Character height multiplier
    float weight;           ///< Character weight multiplier
    
    CharacterAppearance() : faceType(0), hairStyle(0), hairColor(0), eyeColor(0), 
                           skinTone(0), bodyType(0), height(1.0f), weight(1.0f) {}
};

/**
 * @brief SAO Equipment Slots
 * 
 * Defines all possible equipment slots for a player character.
 * Each slot can hold one piece of equipment with specific properties.
 */
enum class EquipmentSlot {
    Weapon,         ///< Primary weapon (sword, axe, etc.)
    Shield,         ///< Off-hand shield or secondary weapon
    Head,           ///< Helmet, hat, or head accessory
    Chest,          ///< Body armor or clothing
    Arms,           ///< Arm guards or sleeves
    Hands,          ///< Gloves or hand protection
    Legs,           ///< Leg armor or pants
    Feet,           ///< Boots or shoes
    Necklace,       ///< Neck accessory or amulet
    Ring1,          ///< First ring slot
    Ring2,          ///< Second ring slot
    Cape,           ///< Back accessory or cape
    Belt,           ///< Waist accessory or belt
    
    MaxSlots
};

/**
 * @brief SAO Item Rarity Levels
 * 
 * Defines the rarity and quality of items in the SAO universe.
 * Higher rarity items have better stats and are more valuable.
 */
enum class ItemRarity {
    Common,         ///< Basic items, easily obtainable
    Uncommon,       ///< Better than common, moderately rare
    Rare,           ///< Valuable items, hard to find
    Unique,         ///< Special items with unique properties
    Legendary,      ///< Extremely rare and powerful items
    Mythical        ///< The rarest items in the game
};

/**
 * @brief SAO Item Base Class
 * 
 * Base class for all items in the SAO universe including
 * weapons, armor, consumables, and materials.
 */
class Item {
public:
    Item(uint32_t id, const std::string& name, ItemRarity rarity);
    virtual ~Item() = default;
    
    // Getters
    uint32_t getId() const { return id_; }
    const std::string& getName() const { return name_; }
    ItemRarity getRarity() const { return rarity_; }
    uint32_t getDurability() const { return durability_; }
    uint32_t getMaxDurability() const { return maxDurability_; }
    uint32_t getValue() const { return value_; }
    
    // Setters
    void setDurability(uint32_t durability) { durability_ = std::min(durability, maxDurability_); }
    void setValue(uint32_t value) { value_ = value; }
    
    // Virtual methods
    virtual bool isEquippable() const = 0;
    virtual bool isConsumable() const = 0;
    virtual bool isStackable() const = 0;
    virtual uint32_t getMaxStack() const { return 1; }
    
    // Durability management
    bool isBroken() const { return durability_ == 0; }
    void repair() { durability_ = maxDurability_; }
    void damage(uint32_t amount);
    
protected:
    uint32_t id_;
    std::string name_;
    ItemRarity rarity_;
    uint32_t durability_;
    uint32_t maxDurability_;
    uint32_t value_;
    std::string description_;
};

/**
 * @brief SAO Equipment Base Class
 * 
 * Base class for all equippable items including weapons and armor.
 * Provides equipment-specific functionality and stat bonuses.
 */
class Equipment : public Item {
public:
    Equipment(uint32_t id, const std::string& name, ItemRarity rarity, EquipmentSlot slot);
    
    bool isEquippable() const override { return true; }
    bool isConsumable() const override { return false; }
    bool isStackable() const override { return false; }
    
    EquipmentSlot getSlot() const { return slot_; }
    const PlayerStats& getStatBonuses() const { return statBonuses_; }
    void setStatBonuses(const PlayerStats& stats) { statBonuses_ = stats; }
    
protected:
    EquipmentSlot slot_;
    PlayerStats statBonuses_;
};

/**
 * @brief SAO Weapon Class
 * 
 * Represents weapons in the SAO universe including swords, axes, bows, etc.
 * Provides weapon-specific properties like damage, attack speed, and range.
 */
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
    
    WeaponType getWeaponType() const { return weaponType_; }
    uint32_t getDamage() const { return damage_; }
    float getAttackSpeed() const { return attackSpeed_; }
    uint32_t getRange() const { return range_; }
    uint32_t getCriticalRate() const { return criticalRate_; }
    
    void setDamage(uint32_t damage) { damage_ = damage; }
    void setAttackSpeed(float speed) { attackSpeed_ = speed; }
    void setRange(uint32_t range) { range_ = range; }
    void setCriticalRate(uint32_t rate) { criticalRate_ = rate; }
    
private:
    WeaponType weaponType_;
    uint32_t damage_;
    float attackSpeed_;
    uint32_t range_;
    uint32_t criticalRate_;
};

/**
 * @brief SAO Armor Class
 * 
 * Represents armor pieces in the SAO universe including helmets, chest pieces, etc.
 * Provides defense bonuses and protection against different damage types.
 */
class Armor : public Equipment {
public:
    enum class ArmorType {
        Light,      ///< Cloth, leather - high mobility, low defense
        Medium,     ///< Chain, scale - balanced mobility and defense
        Heavy       ///< Plate, full armor - low mobility, high defense
    };
    
    Armor(uint32_t id, const std::string& name, ItemRarity rarity, ArmorType type);
    
    ArmorType getArmorType() const { return armorType_; }
    uint32_t getPhysicalDefense() const { return physicalDefense_; }
    uint32_t getMagicalDefense() const { return magicalDefense_; }
    float getWeight() const { return weight_; }
    
    void setPhysicalDefense(uint32_t defense) { physicalDefense_ = defense; }
    void setMagicalDefense(uint32_t defense) { magicalDefense_ = defense; }
    void setWeight(float weight) { weight_ = weight; }
    
private:
    ArmorType armorType_;
    uint32_t physicalDefense_;
    uint32_t magicalDefense_;
    float weight_;
};

/**
 * @brief SAO Player Character Class
 * 
 * Represents a player character in the SAO universe with all associated
 * data including stats, equipment, inventory, and progression.
 */
class PlayerCharacter {
public:
    PlayerCharacter(const std::string& name);
    ~PlayerCharacter() = default;
    
    // Basic information
    const std::string& getName() const { return name_; }
    uint32_t getLevel() const { return level_; }
    uint32_t getExperience() const { return experience_; }
    uint32_t getExperienceToNext() const;
    
    // Stats and progression
    const PlayerStats& getBaseStats() const { return baseStats_; }
    const PlayerStats& getTotalStats() const { return totalStats_; }
    const CharacterAppearance& getAppearance() const { return appearance_; }
    
    void setBaseStats(const PlayerStats& stats);
    void setAppearance(const CharacterAppearance& appearance);
    
    // Leveling and experience
    void addExperience(uint32_t exp);
    void levelUp();
    uint32_t getSkillPoints() const { return skillPoints_; }
    void spendSkillPoints(uint32_t points);
    
    // Health and status
    uint32_t getCurrentHealth() const { return currentHealth_; }
    uint32_t getMaxHealth() const { return maxHealth_; }
    bool isAlive() const { return currentHealth_ > 0; }
    
    void heal(uint32_t amount);
    void damage(uint32_t amount);
    void revive();
    
    // Equipment management
    bool equipItem(std::shared_ptr<Equipment> item);
    std::shared_ptr<Equipment> unequipItem(EquipmentSlot slot);
    std::shared_ptr<Equipment> getEquippedItem(EquipmentSlot slot) const;
    bool isSlotOccupied(EquipmentSlot slot) const;
    
    // Inventory management
    bool addItemToInventory(std::shared_ptr<Item> item);
    bool removeItemFromInventory(uint32_t itemId, uint32_t quantity = 1);
    std::vector<std::shared_ptr<Item>> getInventory() const { return inventory_; }
    uint32_t getInventorySize() const { return inventory_.size(); }
    uint32_t getMaxInventorySize() const { return maxInventorySize_; }
    
    // Currency
    uint32_t getCor() const { return cor_; }
    void addCor(uint32_t amount) { cor_ += amount; }
    bool spendCor(uint32_t amount);
    
    // Utility methods
    void recalculateStats();
    void saveToDatabase();
    void loadFromDatabase();
    
private:
    // Basic information
    std::string name_;
    uint32_t level_;
    uint32_t experience_;
    uint32_t skillPoints_;
    
    // Stats and appearance
    PlayerStats baseStats_;
    PlayerStats totalStats_;
    CharacterAppearance appearance_;
    
    // Health and status
    uint32_t currentHealth_;
    uint32_t maxHealth_;
    
    // Equipment and inventory
    std::map<EquipmentSlot, std::shared_ptr<Equipment>> equippedItems_;
    std::vector<std::shared_ptr<Item>> inventory_;
    uint32_t maxInventorySize_;
    
    // Currency
    uint32_t cor_;
    
    // Private helper methods
    void calculateMaxHealth();
    void updateTotalStats();
    uint32_t calculateExperienceForLevel(uint32_t level) const;
};

/**
 * @brief SAO Character Creation System
 * 
 * Handles the creation of new player characters with validation
 * and customization options.
 */
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
    
private:
    static const uint32_t MAX_NAME_LENGTH = 20;
    static const uint32_t MIN_NAME_LENGTH = 3;
    static const uint32_t MAX_STARTING_STAT_POINTS = 50;
    static const uint32_t MIN_STAT_VALUE = 1;
    static const uint32_t MAX_STAT_VALUE = 20;
};

} // namespace Core
} // namespace SAO 