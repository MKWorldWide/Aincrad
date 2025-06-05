// BossGateRitual.h
// Quantum-detailed header for boss gate ritual
//
// Dependencies: Unreal Engine 5, PlayerCharacter
// Usage: Attach to boss gate Blueprint
// Changelog: Initial scaffold

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossGateRitual.generated.h"

UCLASS(Blueprintable)
class ABossGateRitual : public AActor
{
    GENERATED_BODY()
public:
    ABossGateRitual();
    virtual void BeginPlay() override;
    void CheckRitualRequirements();
}; 