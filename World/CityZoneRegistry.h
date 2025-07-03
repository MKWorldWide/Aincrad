// CityZoneRegistry.h
// Quantum-detailed modular registry/manager for all Eden One City/Hub zones
//
// Feature Context: Central registry for all city hub zones (Unity Spire, Starlake, Districts, etc.)
// Dependencies: Unreal Engine 5, CityZone
// Usage: Register all zones at startup; supports lookup and iteration
// Changelog: Initial scaffold for modular city hub system

#pragma once
#include "CoreMinimal.h"
#include "CityZone.h"
#include "UObject/NoExportTypes.h"
#include "CityZoneRegistry.generated.h"

UCLASS(Blueprintable)
class UCityZoneRegistry : public UObject {
    GENERATED_BODY()
public:
    // Singleton accessor
    UFUNCTION(BlueprintCallable, Category = "ZoneRegistry")
    static UCityZoneRegistry* Get();

    // Register a zone
    UFUNCTION(BlueprintCallable, Category = "ZoneRegistry")
    void RegisterZone(ACityZone* Zone);

    // Get zone by type
    UFUNCTION(BlueprintCallable, Category = "ZoneRegistry")
    ACityZone* GetZoneByType(ECityZoneType ZoneType) const;

    // Get all zones
    UFUNCTION(BlueprintCallable, Category = "ZoneRegistry")
    const TArray<ACityZone*>& GetAllZones() const;

    // Changelog: Real-time updates required
private:
    // All registered zones
    UPROPERTY()
    TArray<ACityZone*> RegisteredZones;

    // Singleton instance
    static UCityZoneRegistry* Instance;
}; 