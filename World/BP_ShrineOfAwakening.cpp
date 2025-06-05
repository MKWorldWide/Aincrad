// BP_ShrineOfAwakening.cpp
// Quantum-detailed implementation for the Shrine of Awakening
//
// Dependencies: Unreal Engine 5, VisionOS
// Usage: Place in world as interactable shrine
// Changelog: Initial scaffold

#include "BP_ShrineOfAwakening.h"
#include "Characters/PlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ABP_ShrineOfAwakening::ABP_ShrineOfAwakening()
{
    PrimaryActorTick.bCanEverTick = true;
    InteractionRange = 300.0f;
    bIsSpatiallyAnchored = true;

    // Create shrine mesh
    ShrineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShrineMesh"));
    RootComponent = ShrineMesh;

    // Create ambient sound
    AmbientSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AmbientSound"));
    AmbientSound->SetupAttachment(RootComponent);
    AmbientSound->bAutoActivate = false;

    // Create shrine particles
    ShrineParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShrineParticles"));
    ShrineParticles->SetupAttachment(RootComponent);
    ShrineParticles->bAutoActivate = false;
}

void ABP_ShrineOfAwakening::BeginPlay()
{
    Super::BeginPlay();
    InitializeShrine();
}

void ABP_ShrineOfAwakening::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bIsSpatiallyAnchored)
    {
        UpdateSpatialAnchor();
    }
}

void ABP_ShrineOfAwakening::OnPlayerApproach(APlayerCharacter* Player)
{
    if (Player)
    {
        // Play ambient sound
        if (AmbientSound && !AmbientSound->IsPlaying())
        {
            AmbientSound->Play();
        }

        // Activate shrine particles
        if (ShrineParticles && !ShrineParticles->IsActive())
        {
            ShrineParticles->Activate(true);
        }

        // Display floating prompt
        // This will be replaced with VisionOS UI
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, TEXT("Touch the stone and speak your name..."));
    }
}

void ABP_ShrineOfAwakening::HandleVoiceInput(const FString& SpokenText)
{
    // Process voice input for player name
    // This will be expanded with actual voice recognition
    PlayerName = SpokenText;
    
    // Play confirmation sound
    if (USoundCue* ConfirmationSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Audio/Shrine/NameConfirmation")))
    {
        UGameplayStatics::PlaySoundAtLocation(this, ConfirmationSound, GetActorLocation());
    }
}

void ABP_ShrineOfAwakening::InitializeShrine()
{
    // Load shrine mesh
    if (UStaticMesh* ShrineMeshAsset = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Meshes/Shrine/ShrineOfAwakening")))
    {
        ShrineMesh->SetStaticMesh(ShrineMeshAsset);
    }

    // Load ambient sound
    if (USoundCue* AmbientSoundAsset = LoadObject<USoundCue>(nullptr, TEXT("/Game/Audio/Shrine/AmbientLoop")))
    {
        AmbientSound->SetSound(AmbientSoundAsset);
    }

    // Load shrine particles
    if (UParticleSystem* ShrineParticlesAsset = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/Particles/Shrine/ShrineAura")))
    {
        ShrineParticles->SetTemplate(ShrineParticlesAsset);
    }
}

void ABP_ShrineOfAwakening::UpdateSpatialAnchor()
{
    // Update spatial anchor position for VisionOS
    // This will be replaced with actual VisionOS spatial anchor logic
} 