// BP_GateOfTruth.h
// Quantum-detailed header for the Gate of Truth
//
// Dependencies: Unreal Engine 5, VisionOS
// Usage: Place in world as ritual gate
// Changelog: Initial scaffold

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BP_GateOfTruth.generated.h"

UCLASS(Blueprintable)
class ABP_GateOfTruth : public AActor
{
    GENERATED_BODY()
public:
    ABP_GateOfTruth();

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* GateMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UParticleSystemComponent* RingParticles;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UAudioComponent* GateSound;

    // Ritual State
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ritual")
    int32 RequiredArtifacts;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ritual")
    int32 PlacedArtifacts;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ritual")
    bool bIsRitualComplete;

    // VisionOS specific
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VisionOS")
    bool bIsSpatiallyAnchored;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Ritual")
    void PlaceArtifact(int32 ArtifactIndex);

    UFUNCTION(BlueprintCallable, Category = "Ritual")
    void CompleteRitual();

private:
    void InitializeGate();
    void UpdateRingParticles();
    void UpdateSpatialAnchor();
}; 