// FloorOneZone.h
// Quantum-detailed header for the first playable floor of Aincrad
//
// Dependencies: Unreal Engine 5, PlayerCharacter
// Usage: Attach to level Blueprint
// Changelog: Initial scaffold

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorOneZone.generated.h"

UCLASS(Blueprintable)
class AFloorOneZone : public AActor
{
    GENERATED_BODY()
public:
    AFloorOneZone();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
}; 