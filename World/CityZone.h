// CityZone.h
// Quantum-detailed modular base class for all Eden One City/Hub zones
//
// Feature Context: Base class for all city hub zones (Unity Spire, Starlake, Districts, etc.)
// Dependencies: Unreal Engine 5, ZoneSystem
// Usage: Inherit for each city zone; register with CityZoneRegistry
// Changelog: Initial scaffold for modular city hub system

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CityZone.generated.h"

UENUM(BlueprintType)
enum class ECityZoneType : uint8 {
    UnitySpire     UMETA(DisplayName = "Unity Spire"),
    Starlake       UMETA(DisplayName = "Starlake"),
    CulturalCrescent UMETA(DisplayName = "Cultural Crescent"),
    EmotionalBiome UMETA(DisplayName = "Emotional Biome Gardens"),
    LoveCreatureHaven UMETA(DisplayName = "Love Creature Haven"),
    MemoryCoastline UMETA(DisplayName = "Memory Coastline"),
    ZeroGDome      UMETA(DisplayName = "Zero-G Dome"),
    StargatePlaza  UMETA(DisplayName = "Stargate Plaza"),
    Interstitial   UMETA(DisplayName = "Interstitial/Connector")
};

UCLASS(Blueprintable)
class ACityZone : public AActor {
    GENERATED_BODY()
public:
    // Zone type (for registry, logic, and visualization)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
    ECityZoneType ZoneType;

    // Human-readable name
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
    FString ZoneName;

    // Connectors (other zones this connects to)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
    TArray<ACityZone*> ConnectedZones;

    // Features (gardens, domes, etc.)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
    TArray<FString> Features;

    // Constructor
    ACityZone();

    // Virtual methods for initialization and interaction
    virtual void InitializeZone();
    virtual void OnPlayerEnter(class APlayerCharacter* Player);
    virtual void OnPlayerExit(class APlayerCharacter* Player);

    // Quantum-detailed inline documentation auto-maintained by AI
    // Usage Example: See UnitySpireZone.h for subclassing
    // Performance: Designed for modular, efficient zone management
    // Security: Validate connectors and player transitions
    // Changelog: Real-time updates required
}; 