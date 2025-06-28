// BP_AincradPlayer.h
// Quantum-detailed header for Aincrad player character with Vision Pro support
//
// Dependencies: Unreal Engine 5, PlayerCharacter, VisionOS
// Usage: Base class for player character with gesture controls
// Changelog: Initial scaffold

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BP_AincradPlayer.generated.h"

UCLASS(Blueprintable)
class ABP_AincradPlayer : public ACharacter
{
    GENERATED_BODY()
public:
    ABP_AincradPlayer();

    // Vision Pro Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vision Pro")
    class UParticleSystemComponent* HandAuraEffect;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vision Pro")
    class USceneComponent* GestureTrackingPoint;

    // Vision Pro Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vision Pro")
    bool bIsGestureActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vision Pro")
    FVector GesturePosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vision Pro")
    FVector GestureDirection;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Vision Pro Functions
    UFUNCTION(BlueprintCallable, Category = "Vision Pro")
    void HandleGestureInput(const FVector& Position, const FVector& Direction);

    UFUNCTION(BlueprintCallable, Category = "Vision Pro")
    void UpdateHandAura(const FVector& Position);

private:
    void InitializeHandAura();
    void PlayWelcomeWhisper();
}; 