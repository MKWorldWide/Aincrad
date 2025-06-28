#pragma once

#include "../Core/PlayerSystem.h"
#include <vector>
#include <map>
#include <memory>
#include <chrono>
#include <functional>

namespace SAO {
namespace Combat {

/**
 * @brief SAO Combat Stance
 * 
 * Defines different combat stances that affect attack patterns,
 * defense, and movement capabilities.
 */
enum class CombatStance {
    Normal,         ///< Balanced stance with standard capabilities
    Offensive,      ///< Aggressive stance with increased attack power
    Defensive,      ///< Defensive stance with increased defense
    Evasive,        ///< Mobile stance with increased dodge rate
    Berserker       ///< High-risk stance with maximum attack power
};

/**
 * @brief SAO Damage Type
 * 
 * Different types of damage that can be dealt and resisted.
 */
enum class DamageType {
    Physical,       ///< Standard physical damage
    Slashing,       ///< Sword, axe damage
    Piercing,       ///< Spear, dagger damage
    Blunt,          ///< Mace, hammer damage
    Magical,        ///< Magic-based damage
    Fire,           ///< Fire elemental damage
    Ice,            ///< Ice elemental damage
    Lightning,      ///< Lightning elemental damage
    Holy,           ///< Holy/divine damage
    Dark            ///< Dark/shadow damage
};

/**
 * @brief SAO Combat Result
 * 
 * Represents the result of a combat action including
 * damage dealt, critical hits, and special effects.
 */
struct CombatResult {
    bool hit;                   ///< Whether the attack hit
    bool critical;              ///< Whether it was a critical hit
    uint32_t damage;            ///< Total damage dealt
    DamageType damageType;      ///< Type of damage dealt
    bool blocked;               ///< Whether the attack was blocked
    bool parried;               ///< Whether the attack was parried
    bool dodged;                ///< Whether the attack was dodged
    std::vector<std::string> effects; ///< Special effects applied
    
    CombatResult() : hit(false), critical(false), damage(0), 
                     damageType(DamageType::Physical), blocked(false), 
                     parried(false), dodged(false) {}
};

/**
 * @brief SAO Sword Skill Base Class
 * 
 * Base class for all sword skills in the SAO universe.
 * Sword skills are special combat techniques that can be learned and mastered.
 */
class SwordSkill {
public:
    enum class SkillType {
        Basic,          ///< Basic sword skills (Horizontal, Vertical, Diagonal)
        Advanced,       ///< Advanced sword skills (Linear, Arc, Spiral)
        Unique,         ///< Unique sword skills (Starburst Stream, Vorpal Strike)
        Ultimate        ///< Ultimate sword skills (The Eclipse, etc.)
    };
    
    SwordSkill(uint32_t id, const std::string& name, SkillType type, uint32_t cooldown);
    virtual ~SwordSkill() = default;
    
    // Getters
    uint32_t getId() const { return id_; }
    const std::string& getName() const { return name_; }
    SkillType getType() const { return type_; }
    uint32_t getCooldown() const { return cooldown_; }
    uint32_t getLevel() const { return level_; }
    uint32_t getMaxLevel() const { return maxLevel_; }
    
    // Skill progression
    void levelUp();
    bool canLevelUp() const;
    uint32_t getExperienceToNext() const;
    void addExperience(uint32_t exp);
    
    // Combat usage
    bool canUse() const;
    void use();
    virtual CombatResult execute(std::shared_ptr<Core::PlayerCharacter> user, 
                                std::shared_ptr<Core::PlayerCharacter> target) = 0;
    
    // Cooldown management
    bool isOnCooldown() const;
    uint32_t getRemainingCooldown() const;
    void resetCooldown();
    
protected:
    uint32_t id_;
    std::string name_;
    SkillType type_;
    uint32_t cooldown_;
    uint32_t level_;
    uint32_t maxLevel_;
    uint32_t experience_;
    uint32_t experienceToNext_;
    std::chrono::steady_clock::time_point lastUsed_;
    
    // Helper methods
    uint32_t calculateExperienceForLevel(uint32_t level) const;
    void updateExperienceToNext();
};

/**
 * @brief SAO Basic Sword Skill
 * 
 * Basic sword skills that all players can learn and use.
 * These form the foundation of combat in SAO.
 */
class BasicSwordSkill : public SwordSkill {
public:
    enum class BasicType {
        Horizontal,     ///< Horizontal slash
        Vertical,       ///< Vertical slash
        Diagonal,       ///< Diagonal slash
        Thrust,         ///< Forward thrust
        Guard           ///< Defensive guard
    };
    
    BasicSwordSkill(uint32_t id, const std::string& name, BasicType type);
    
    BasicType getBasicType() const { return basicType_; }
    CombatResult execute(std::shared_ptr<Core::PlayerCharacter> user, 
                        std::shared_ptr<Core::PlayerCharacter> target) override;
    
private:
    BasicType basicType_;
    uint32_t baseDamage_;
    float accuracy_;
};

/**
 * @brief SAO Advanced Sword Skill
 * 
 * Advanced sword skills that require specific conditions to learn.
 * These provide more powerful combat options.
 */
class AdvancedSwordSkill : public SwordSkill {
public:
    enum class AdvancedType {
        Linear,         ///< Linear attack pattern
        Arc,            ///< Arc-shaped attack pattern
        Spiral,         ///< Spiral attack pattern
        Cross,          ///< Cross-shaped attack pattern
        Star            ///< Star-shaped attack pattern
    };
    
    AdvancedSwordSkill(uint32_t id, const std::string& name, AdvancedType type);
    
    AdvancedType getAdvancedType() const { return advancedType_; }
    CombatResult execute(std::shared_ptr<Core::PlayerCharacter> user, 
                        std::shared_ptr<Core::PlayerCharacter> target) override;
    
private:
    AdvancedType advancedType_;
    uint32_t baseDamage_;
    float accuracy_;
    uint32_t comboHits_;
};

/**
 * @brief SAO Unique Sword Skill
 * 
 * Unique sword skills that are signature moves of specific characters
 * or require special conditions to obtain.
 */
class UniqueSwordSkill : public SwordSkill {
public:
    UniqueSwordSkill(uint32_t id, const std::string& name, uint32_t cooldown);
    
    CombatResult execute(std::shared_ptr<Core::PlayerCharacter> user, 
                        std::shared_ptr<Core::PlayerCharacter> target) override = 0;
    
protected:
    virtual uint32_t calculateDamage(std::shared_ptr<Core::PlayerCharacter> user) = 0;
    virtual std::vector<std::string> getSpecialEffects() = 0;
};

/**
 * @brief SAO Starburst Stream Skill
 * 
 * Kirito's signature 16-hit combo skill.
 * One of the most powerful unique sword skills in SAO.
 */
class StarburstStream : public UniqueSwordSkill {
public:
    StarburstStream();
    
    CombatResult execute(std::shared_ptr<Core::PlayerCharacter> user, 
                        std::shared_ptr<Core::PlayerCharacter> target) override;
    
protected:
    uint32_t calculateDamage(std::shared_ptr<Core::PlayerCharacter> user) override;
    std::vector<std::string> getSpecialEffects() override;
    
private:
    static const uint32_t COMBO_HITS = 16;
    static const uint32_t BASE_DAMAGE_PER_HIT = 50;
};

/**
 * @brief SAO Vorpal Strike Skill
 * 
 * A powerful single-hit skill with high critical rate.
 * Effective against heavily armored opponents.
 */
class VorpalStrike : public UniqueSwordSkill {
public:
    VorpalStrike();
    
    CombatResult execute(std::shared_ptr<Core::PlayerCharacter> user, 
                        std::shared_ptr<Core::PlayerCharacter> target) override;
    
protected:
    uint32_t calculateDamage(std::shared_ptr<Core::PlayerCharacter> user) override;
    std::vector<std::string> getSpecialEffects() override;
    
private:
    static const uint32_t BASE_DAMAGE = 200;
    static const uint32_t CRITICAL_BONUS = 100;
};

/**
 * @brief SAO Combat System
 * 
 * Manages all combat-related functionality including
 * turn-based and real-time combat, skill usage, and battle resolution.
 */
class CombatSystem {
public:
    CombatSystem();
    ~CombatSystem() = default;
    
    // Combat initialization
    void initializeCombat(std::shared_ptr<Core::PlayerCharacter> player1,
                         std::shared_ptr<Core::PlayerCharacter> player2);
    void initializeCombat(std::vector<std::shared_ptr<Core::PlayerCharacter>> players);
    
    // Combat execution
    CombatResult performAttack(std::shared_ptr<Core::PlayerCharacter> attacker,
                              std::shared_ptr<Core::PlayerCharacter> target);
    CombatResult useSwordSkill(std::shared_ptr<Core::PlayerCharacter> user,
                              std::shared_ptr<Core::PlayerCharacter> target,
                              std::shared_ptr<SwordSkill> skill);
    
    // Combat state
    bool isCombatActive() const { return combatActive_; }
    std::vector<std::shared_ptr<Core::PlayerCharacter>> getCombatants() const { return combatants_; }
    std::shared_ptr<Core::PlayerCharacter> getCurrentTurn() const { return currentTurn_; }
    
    // Combat flow
    void startCombat();
    void endCombat();
    void nextTurn();
    
    // Combat mechanics
    bool canAttack(std::shared_ptr<Core::PlayerCharacter> attacker,
                   std::shared_ptr<Core::PlayerCharacter> target) const;
    uint32_t calculateDamage(std::shared_ptr<Core::PlayerCharacter> attacker,
                            std::shared_ptr<Core::PlayerCharacter> target,
                            DamageType damageType = DamageType::Physical);
    bool checkHit(std::shared_ptr<Core::PlayerCharacter> attacker,
                  std::shared_ptr<Core::PlayerCharacter> target) const;
    bool checkCritical(std::shared_ptr<Core::PlayerCharacter> attacker) const;
    bool checkBlock(std::shared_ptr<Core::PlayerCharacter> target) const;
    bool checkParry(std::shared_ptr<Core::PlayerCharacter> target) const;
    bool checkDodge(std::shared_ptr<Core::PlayerCharacter> target) const;
    
    // Skill management
    void registerSwordSkill(std::shared_ptr<SwordSkill> skill);
    std::vector<std::shared_ptr<SwordSkill>> getAvailableSkills(std::shared_ptr<Core::PlayerCharacter> user) const;
    
    // Combat events
    using CombatEventHandler = std::function<void(const CombatResult&)>;
    void addCombatEventHandler(CombatEventHandler handler);
    void removeCombatEventHandler(CombatEventHandler handler);
    
private:
    bool combatActive_;
    std::vector<std::shared_ptr<Core::PlayerCharacter>> combatants_;
    std::shared_ptr<Core::PlayerCharacter> currentTurn_;
    std::vector<std::shared_ptr<SwordSkill>> registeredSkills_;
    std::vector<CombatEventHandler> eventHandlers_;
    
    // Private helper methods
    void triggerCombatEvent(const CombatResult& result);
    uint32_t calculateBaseDamage(std::shared_ptr<Core::PlayerCharacter> attacker) const;
    float calculateAccuracy(std::shared_ptr<Core::PlayerCharacter> attacker) const;
    float calculateDodgeRate(std::shared_ptr<Core::PlayerCharacter> target) const;
    float calculateBlockRate(std::shared_ptr<Core::PlayerCharacter> target) const;
    float calculateParryRate(std::shared_ptr<Core::PlayerCharacter> target) const;
    uint32_t calculateCriticalRate(std::shared_ptr<Core::PlayerCharacter> attacker) const;
    uint32_t calculateCriticalDamage(std::shared_ptr<Core::PlayerCharacter> attacker) const;
};

/**
 * @brief SAO Combat Manager
 * 
 * Global manager for all combat-related systems and data.
 * Provides access to skill databases, combat configurations, and global combat state.
 */
class CombatManager {
public:
    static CombatManager& getInstance();
    
    // Skill database
    void loadSkillDatabase();
    std::shared_ptr<SwordSkill> getSkillById(uint32_t id) const;
    std::shared_ptr<SwordSkill> getSkillByName(const std::string& name) const;
    std::vector<std::shared_ptr<SwordSkill>> getAllSkills() const;
    std::vector<std::shared_ptr<SwordSkill>> getSkillsByType(SwordSkill::SkillType type) const;
    
    // Combat configuration
    void setCombatMode(bool realTime);
    bool isRealTimeCombat() const { return realTimeCombat_; }
    
    void setTurnTimeLimit(uint32_t milliseconds);
    uint32_t getTurnTimeLimit() const { return turnTimeLimit_; }
    
    // Global combat state
    std::vector<std::shared_ptr<CombatSystem>> getActiveCombats() const;
    void registerCombat(std::shared_ptr<CombatSystem> combat);
    void unregisterCombat(std::shared_ptr<CombatSystem> combat);
    
private:
    CombatManager() = default;
    ~CombatManager() = default;
    CombatManager(const CombatManager&) = delete;
    CombatManager& operator=(const CombatManager&) = delete;
    
    std::map<uint32_t, std::shared_ptr<SwordSkill>> skillDatabase_;
    std::map<std::string, std::shared_ptr<SwordSkill>> skillNameMap_;
    std::vector<std::shared_ptr<CombatSystem>> activeCombats_;
    bool realTimeCombat_;
    uint32_t turnTimeLimit_;
};

} // namespace Combat
} // namespace SAO 