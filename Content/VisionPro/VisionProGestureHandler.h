// VisionProGestureHandler.h
// Quantum-detailed header for Vision Pro gesture handling
//
// Dependencies: Unreal Engine 5, VisionOS
// Usage: Handles Vision Pro specific gestures
// Changelog: Initial scaffold

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VisionProGestureHandler.generated.h"

UENUM(BlueprintType)
enum class EVisionProGestureType : uint8
{
    None,
    Pinch,
    Grab,
    Point,
    Wave,
    Custom
};

USTRUCT(BlueprintType)
struct FVisionProGestureData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EVisionProGestureType GestureType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Position;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Direction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Confidence;
};

UCLASS(Blueprintable)
class AVisionProGestureHandler : public AActor
{
    GENERATED_BODY()
public:
    AVisionProGestureHandler();

    // Gesture Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gestures")
    float GestureSensitivity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gestures")
    float GestureConfidenceThreshold;

    // Gesture Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGestureDetected, const FVisionProGestureData&, GestureData);
    UPROPERTY(BlueprintAssignable, Category = "Gestures")
    FOnGestureDetected OnGestureDetected;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Gestures")
    void ProcessGestureInput(const FVector& Position, const FVector& Direction);

    UFUNCTION(BlueprintCallable, Category = "Gestures")
    EVisionProGestureType RecognizeGesture(const FVector& Position, const FVector& Direction);

private:
    void InitializeGestureRecognition();
    float CalculateGestureConfidence(const FVector& Position, const FVector& Direction);
}; 