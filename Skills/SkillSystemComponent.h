// SkillSystemComponent.h
// Quantum-detailed header for MMORPG skill system
//
// Dependencies: Unreal Engine 5
// Usage: Attach to player or enemy character
// Changelog: Initial scaffold

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillSystemComponent.generated.h"

USTRUCT(BlueprintType)
struct FSkill
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
    FString Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
    float Cooldown;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
    float EnergyCost;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
    bool bIsOnCooldown;
    // Animation, effect, etc.
    FSkill();
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class USkillSystemComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    USkillSystemComponent();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
    TArray<FSkill> Skills;
    UFUNCTION(BlueprintCallable, Category = Skills)
    void InitializeSkills();
    UFUNCTION(BlueprintCallable, Category = Skills)
    void ActivateSkill(int32 SkillIndex);
    // ...
}; 