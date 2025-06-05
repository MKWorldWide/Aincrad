// WeaponComponent.h
// Quantum-detailed header for MMORPG weapon logic
//
// Dependencies: Unreal Engine 5, SkillSystemComponent
// Usage: Attach to player or enemy character
// Changelog: Initial scaffold

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Sword UMETA(DisplayName = "Sword"),
    Spear UMETA(DisplayName = "Spear"),
    DualBlades UMETA(DisplayName = "Dual Blades")
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UWeaponComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UWeaponComponent();
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
    EWeaponType WeaponType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
    int32 MaxDurability;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
    int32 Durability;
    UPROPERTY(BlueprintReadOnly, Category = Combo)
    int32 ComboIndex;

    UFUNCTION(BlueprintCallable, Category = Weapon)
    void Attack(bool bIsHeavy);
    UFUNCTION(BlueprintCallable, Category = Weapon)
    void SwitchWeapon(EWeaponType NewType);

    // ...
}; 