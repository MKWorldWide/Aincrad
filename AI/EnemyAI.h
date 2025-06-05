// EnemyAI.h
// Quantum-detailed header for MMORPG enemy AI
//
// Dependencies: Unreal Engine 5, ZoneSystem
// Usage: Base class for enemy character Blueprint
// Changelog: Initial scaffold

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyAI.generated.h"

UCLASS(Blueprintable)
class AEnemyAI : public AActor
{
    GENERATED_BODY()
public:
    AEnemyAI();
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
    float AggroRange;
    UPROPERTY(BlueprintReadOnly, Category = AI)
    bool bIsAggroed;
    // Patrol, attack, retreat, weak spot logic
    // ...
}; 