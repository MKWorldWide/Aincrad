// VisionProVoiceHandler.h
// Quantum-detailed header for Vision Pro voice handling
//
// Dependencies: Unreal Engine 5, VisionOS
// Usage: Handles Vision Pro specific voice input
// Changelog: Initial scaffold

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VisionProVoiceHandler.generated.h"

UENUM(BlueprintType)
enum class EVisionProVoiceCommand : uint8
{
    None,
    Attack,
    Defend,
    Cast,
    Interact,
    Menu,
    Custom
};

USTRUCT(BlueprintType)
struct FVisionProVoiceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EVisionProVoiceCommand Command;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SpokenText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Confidence;
};

UCLASS(Blueprintable)
class AVisionProVoiceHandler : public AActor
{
    GENERATED_BODY()
public:
    AVisionProVoiceHandler();

    // Voice Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice")
    float VoiceRecognitionThreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice")
    bool bEnableVoiceCommands;

    // Voice Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVoiceCommandDetected, const FVisionProVoiceData&, VoiceData);
    UPROPERTY(BlueprintAssignable, Category = "Voice")
    FOnVoiceCommandDetected OnVoiceCommandDetected;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Voice")
    void ProcessVoiceInput(const FString& SpokenText);

    UFUNCTION(BlueprintCallable, Category = "Voice")
    EVisionProVoiceCommand RecognizeVoiceCommand(const FString& SpokenText);

private:
    void InitializeVoiceRecognition();
    float CalculateVoiceConfidence(const FString& SpokenText);
}; 