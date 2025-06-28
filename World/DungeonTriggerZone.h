// DungeonTriggerZone.h
// Quantum-detailed header for dungeon trigger zone
//
// Dependencies: Unreal Engine 5, PlayerCharacter
// Usage: Attach to dungeon gate Blueprint
// Changelog: Initial scaffold

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonTriggerZone.generated.h"

UCLASS(Blueprintable)
class ADungeonTriggerZone : public AActor
{
    GENERATED_BODY()
public:
    ADungeonTriggerZone();
    virtual void BeginPlay() override;
    void OnPlayerApproach();
}; 