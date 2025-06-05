// BP_AincradPlayer.h
// Quantum-detailed header for Aincrad player character with gesture support
//
// Dependencies: Unreal Engine 5, PlayerCharacter, VisionOS
// Usage: Base class for player character with gesture controls
// Changelog: Initial scaffold

#pragma once
#include "CoreMinimal.h"
#include "Characters/PlayerCharacter.h"
#include "BP_AincradPlayer.generated.h"

UCLASS(Blueprintable)
class ABP_AincradPlayer : public APlayerCharacter
{
    GENERATED_BODY()
public:
    ABP_AincradPlayer();

    // Gesture Input
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VisionOS|Gestures")
    bool bIsGestureActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VisionOS|Effects")
    class UParticleSystemComponent* HandAuraEffect;

    // VisionOS specific functions
    UFUNCTION(BlueprintCallable, Category = "VisionOS|Gestures")
    void HandleGestureInput(const FVector& GesturePosition, const FVector& GestureDirection);

    UFUNCTION(BlueprintCallable, Category = "VisionOS|Effects")
    void UpdateHandAura(const FVector& HandPosition);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    void InitializeHandAura();
    void PlayWelcomeWhisper();
}; 