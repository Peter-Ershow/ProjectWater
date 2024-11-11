// PlayerPawnView.cpp

#include "CPP_PlayerPawnView.h"

#include "CPP_PlayerPawnController.h"
#include "Components/AudioComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/World.h"
#include "ProjectWater/ProjectWater.h"
#include "ProjectWater/Actors/CPP_Screenfade.h"

ACPP_PlayerPawnView::ACPP_PlayerPawnView()
{
    PrimaryActorTick.bCanEverTick = true;
    
    MusicAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MusicAudioComponent"));
    MusicAudioComponent->SetupAttachment(RootComponent);

    SFXAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SFXAudioComponent"));
    SFXAudioComponent->SetupAttachment(RootComponent);

    GrabAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("GrabAudioComponent"));
    GrabAudioComponent->SetupAttachment(RootComponent);
    
    ScreenFadeTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ScreenFadeTimeline"));

    PlayerController = Cast<ACPP_PlayerPawnController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

    if(!PlayerController)
    {
        UE_LOG(LogProjectWater, Error, TEXT("No Player Controller found"));
    }

}

void ACPP_PlayerPawnView::BeginPlay()
{
    Super::BeginPlay();
    InitializeAudioComponents();
    InitializeScreenFadeComponents();
}

void ACPP_PlayerPawnView::InitializeAudioComponents()
{
    if (MusicAudioComponent)
    {
        MusicAudioComponent->OnAudioFinished.AddDynamic(this, &ACPP_PlayerPawnView::OnMusicSoundFinished);
    }

    if (SFXAudioComponent)
    {
        SFXAudioComponent->OnAudioFinished.AddDynamic(this, &ACPP_PlayerPawnView::OnSFXSoundFinished);
    }

    if (GrabAudioComponent)
    {
        GrabAudioComponent->OnAudioFinished.AddDynamic(this, &ACPP_PlayerPawnView::OnGrabSoundFinished);
    }
}

void ACPP_PlayerPawnView::InitializeScreenFadeComponents()
{
    if (FadeCurve && ScreenFadeTimeline)
    {
        FOnTimelineFloat ProgressFunction;
        ProgressFunction.BindUFunction(this, FName("UpdateScreenFade"));
        ScreenFadeTimeline->AddInterpFloat(FadeCurve, ProgressFunction);

        FOnTimelineEvent TimelineFinished;
        TimelineFinished.BindUFunction(this, FName("OnFadeFinished"));
        ScreenFadeTimeline->SetTimelineFinishedFunc(TimelineFinished);
    }
}

void ACPP_PlayerPawnView::PlaySoundGrabComponent(USoundBase* InteractionSound, float Volume)
{
    if (GrabAudioComponent && InteractionSound)
    {
        GrabAudioComponent->SetSound(InteractionSound);
        GrabAudioComponent->SetVolumeMultiplier(Volume);
        GrabAudioComponent->Play();
    }
}

void ACPP_PlayerPawnView::PlaySoundSFXComponent(USoundBase* InteractionSound, float Volume)
{
    if (SFXAudioComponent && InteractionSound)
    {
        SFXAudioComponent->SetSound(InteractionSound);
        SFXAudioComponent->SetVolumeMultiplier(Volume);
        SFXAudioComponent->Play();
    }
}

void ACPP_PlayerPawnView::StopSoundGrabComponent()
{
    if (GrabAudioComponent)
    {
        GrabAudioComponent->Stop();
    }
}

void ACPP_PlayerPawnView::StopSoundSFXComponent()
{
    if (SFXAudioComponent)
    {
        SFXAudioComponent->Stop();
    }
}

void ACPP_PlayerPawnView::FadeOutSoundSFXComponent()
{
    if (SFXAudioComponent)
    {
        SFXAudioComponent->FadeOut(0.5, 1);
    }
}


void ACPP_PlayerPawnView::FadeToScenePawn(bool bOpenALevel)
{
    if (const APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0))
    {
        if (ScreenFadeActor)
        {
            const FTransform SpawnTransform = CameraManager->GetRootComponent()->GetComponentTransform();
            FActorSpawnParameters SpawnParameters;
            SpawnParameters.Owner = this;
            SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            // Spawn the screen fade actor(black sphere)
            SpawnedActor = GetWorld()->SpawnActor<ACPP_Screenfade>(ScreenFadeActor, SpawnTransform, SpawnParameters);
            if (SpawnedActor && SpawnedActor->Sphere)
            {
                if (UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(SpawnedActor->Sphere->GetMaterial(0), this))
                {
                    SpawnedActor->Sphere->SetMaterial(0, DynamicMaterialInstance);
                    DynamicMaterialInstance->SetVectorParameterValue(FName("ScreenFadeColor"), FLinearColor::Black);
                    DynamicMaterialInstance->SetScalarParameterValue(FName("OpacityColor"), 0.5f); // Initial opacity

                    // Start the timeline or reverse a timeline if it is set up
                    if (ScreenFadeTimeline)
                    {
                        if(bOpenALevel)
                        {
                            ScreenFadeTimeline->PlayFromStart();
                        }
                        else
                        {
                            ScreenFadeTimeline->ReverseFromEnd();
                        }
                    }
                }
            }
        }
    }
}

void ACPP_PlayerPawnView::UpdateScreenFade(float Value, UMaterialInstanceDynamic* DynamicMaterial)
{
    if (DynamicMaterial)
    {
        DynamicMaterial->SetScalarParameterValue(FName("OpacityColor"), Value);
    }
}

void ACPP_PlayerPawnView::OnFadeFinished()
{
    GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
    {
        GetWorld()->GetTimerManager().SetTimer(
            ScreenFadeTimerHandle,
            this,
            &ACPP_PlayerPawnView::DestroyAfterDelay, 
            3.0f, 
            false
        );
    });
}

void ACPP_PlayerPawnView::OnGrabSoundFinished()
{
    PlayerController->OnStopGrab();
}

void ACPP_PlayerPawnView::OnSFXSoundFinished()
{
    PlayerController->OnStopSFX();
}

void ACPP_PlayerPawnView::OnMusicSoundFinished()
{
    if (MusicAudioComponent && MusicAudioComponent->Sound)
    {
        MusicAudioComponent->Play();
    }
}

void ACPP_PlayerPawnView::DestroyAfterDelay()
{
    SpawnedActor->Destroy();
}
