// BP_ShrineOfAwakening.h
// Quantum-detailed header for the Shrine of Awakening
//
// Dependencies: Unreal Engine 5, VisionOS
// Usage: Place in world as interactable shrine
// Changelog: Initial scaffold

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BP_ShrineOfAwakening.generated.h"

UCLASS(Blueprintable)
class ABP_ShrineOfAwakening : public AActor
{
    GENERATED_BODY()
public:
    ABP_ShrineOfAwakening();

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* ShrineMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UAudioComponent* AmbientSound;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UParticleSystemComponent* ShrineParticles;

    // Interaction
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    float InteractionRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FString PlayerName;

    // VisionOS specific
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VisionOS")
    bool bIsSpatiallyAnchored;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void OnPlayerApproach(class APlayerCharacter* Player);

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void HandleVoiceInput(const FString& SpokenText);

private:
    void InitializeShrine();
    void UpdateSpatialAnchor();
}; 