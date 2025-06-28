// HUDSystem.h
// Quantum-detailed header for MMORPG HUD system
//
// Dependencies: Unreal Engine 5, SkillSystemComponent
// Usage: Attach to player HUD Blueprint
// Changelog: Initial scaffold

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDSystem.generated.h"

UCLASS(Blueprintable)
class UHUDSystem : public AHUD
{
    GENERATED_BODY()
public:
    UHUDSystem();
    UFUNCTION(BlueprintCallable, Category = HUD)
    void DrawHUD();
    // Health, stamina, skill cooldowns, party UI
    // ...
}; 