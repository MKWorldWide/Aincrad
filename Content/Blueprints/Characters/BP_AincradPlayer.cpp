// BP_AincradPlayer.cpp
// Quantum-detailed implementation for Aincrad player character with Vision Pro support
//
// Dependencies: Unreal Engine 5, PlayerCharacter, VisionOS
// Usage: Base class for player character with gesture controls
// Changelog: Initial scaffold

#include "BP_AincradPlayer.h"
#include "Components/ParticleSystemComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ABP_AincradPlayer::ABP_AincradPlayer()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsGestureActive = false;

    // Create gesture tracking point
    GestureTrackingPoint = CreateDefaultSubobject<USceneComponent>(TEXT("GestureTrackingPoint"));
    GestureTrackingPoint->SetupAttachment(RootComponent);
    GestureTrackingPoint->SetRelativeLocation(FVector(0, 0, 100));

    // Create hand aura particle system
    HandAuraEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HandAuraEffect"));
    HandAuraEffect->SetupAttachment(GestureTrackingPoint);
    HandAuraEffect->bAutoActivate = false;
}

void ABP_AincradPlayer::BeginPlay()
{
    Super::BeginPlay();
    InitializeHandAura();
    PlayWelcomeWhisper();
}

void ABP_AincradPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bIsGestureActive)
    {
        UpdateHandAura(GesturePosition);
    }
}

void ABP_AincradPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    // Bind gesture input events
}

void ABP_AincradPlayer::HandleGestureInput(const FVector& Position, const FVector& Direction)
{
    GesturePosition = Position;
    GestureDirection = Direction;
    bIsGestureActive = true;

    // Process gesture for spell casting, item interaction, etc.
    // This will be expanded with actual VisionOS gesture recognition
}

void ABP_AincradPlayer::UpdateHandAura(const FVector& Position)
{
    if (HandAuraEffect)
    {
        HandAuraEffect->SetWorldLocation(Position);
    }
}

void ABP_AincradPlayer::InitializeHandAura()
{
    if (HandAuraEffect)
    {
        // Load and set particle system
        if (UParticleSystem* AuraParticles = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/Particles/Player/HandAura")))
        {
            HandAuraEffect->SetTemplate(AuraParticles);
            HandAuraEffect->Activate(true);
        }
    }
}

void ABP_AincradPlayer::PlayWelcomeWhisper()
{
    // Play welcome whisper sound effect
    if (USoundCue* WelcomeSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Audio/Player/WelcomeWhisper")))
    {
        UGameplayStatics::PlaySoundAtLocation(this, WelcomeSound, GetActorLocation());
    }
} 