// BP_WelcomeSequence.h
// Quantum-detailed header for the Welcome Sequence
//
// Dependencies: Unreal Engine 5, VisionOS
// Usage: Manages the initial welcome sequence for new players
// Changelog: Initial scaffold

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BP_WelcomeSequence.generated.h"

UCLASS(Blueprintable)
class ABP_WelcomeSequence : public AActor
{
    GENERATED_BODY()
public:
    ABP_WelcomeSequence();

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USceneComponent* Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UAudioComponent* WelcomeAudio;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UParticleSystemComponent* WelcomeEffects;

    // Sequence properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence")
    float SequenceDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence")
    FString PlayerName;

    // Vision Pro specific
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vision Pro")
    bool bUseSpatialAudio;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Sequence")
    void StartWelcomeSequence();

    UFUNCTION(BlueprintCallable, Category = "Sequence")
    void HandlePlayerResponse(const FString& Response);

private:
    void InitializeSequence();
    void UpdateSpatialAudio();
    void PlayWelcomeEffects();
}; 