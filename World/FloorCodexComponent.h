// FloorCodexComponent.h
// Quantum-detailed header for floor codex component
//
// Dependencies: Unreal Engine 5, PlayerCharacter
// Usage: Attach to player or enemy character
// Changelog: Initial scaffold

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FloorCodexComponent.generated.h"

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UFloorCodexComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UFloorCodexComponent();
    virtual void BeginPlay() override;
    void LogEvent(const FString& EventType);
}; 