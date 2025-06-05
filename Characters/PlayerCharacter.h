// PlayerCharacter.h
// Quantum-detailed header for MMORPG third-person character
//
// Dependencies: Unreal Engine 5, WeaponComponent, SkillSystemComponent
// Usage: Base class for player character Blueprint
// Changelog: Initial scaffold

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UWeaponComponent;
class USkillSystemComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS(Blueprintable)
class APlayerCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    APlayerCharacter();
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Camera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    USpringArmComponent* CameraBoom;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    UCameraComponent* FollowCamera;

    // Combat
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
    UWeaponComponent* WeaponComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skills)
    USkillSystemComponent* SkillSystemComponent;

    // Stamina
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
    float MaxStamina;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
    float Stamina;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
    float StaminaRegenRate;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
    float SprintStaminaCost;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
    float DodgeStaminaCost;

    UPROPERTY(BlueprintReadOnly, Category = State)
    bool bIsSprinting;
    UPROPERTY(BlueprintReadOnly, Category = State)
    bool bIsLockedOn;

    // Movement, attack, lock-on, sprint, dodge, stamina methods
    // ...
}; 