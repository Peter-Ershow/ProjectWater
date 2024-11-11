#include "CPP_PlayerPawnController.h"

#include "CPP_PlayerPawnModel.h"
#include "CPP_PlayerPawnView.h"
#include "Camera/CameraComponent.h"
#include "Sound/SoundBase.h"
#include "Engine/World.h"
#include "ProjectWater/ProjectWater.h"

ACPP_PlayerPawnController::ACPP_PlayerPawnController()
{
    PawnModel = GetPlayerState<ACPP_PlayerPawnModel>();

    if(!PawnModel)
    {
        UE_LOG(LogProjectWater, Error, TEXT("PlayerPawnView not found on the controlled pawn."));
    }

    PawnView = Cast<ACPP_PlayerPawnView>(GetPawn());
    if (!PawnView)
    {
        UE_LOG(LogProjectWater, Error, TEXT("PlayerPawnView not found on the controlled pawn."));
    }
}

void ACPP_PlayerPawnController::BeginPlay()
{
    Super::BeginPlay();
}

void ACPP_PlayerPawnController::SetupInputComponent()
{
    Super::SetupInputComponent();
}

void ACPP_PlayerPawnController::VerifyAndPlayGrabComponent(USoundBase* InteractionSound, float Volume)
{
    if (PawnView && PawnModel && PawnModel->canListenToGrabSounds)
    {
        PawnView->PlaySoundGrabComponent(InteractionSound, Volume); 
        PawnModel->canListenToGrabSounds = false;
    }
}

void ACPP_PlayerPawnController::OnGrab(EControllerHand NewHeldHand)
{
    PawnModel->HeldHand = NewHeldHand;
    OnGrabbedEvent.Broadcast();
}

void ACPP_PlayerPawnController::VerifyAndPlaySFXComponent(USoundBase* InteractionSound, float Volume)
{
    if (PawnView && PawnModel && PawnModel->canListenToSounds)
    {
        PawnView->PlaySoundSFXComponent(InteractionSound, Volume);
        PawnModel->canListenToSounds = false;
    }
}

void ACPP_PlayerPawnController::StopSoundGrabComponent()
{
    PawnView->StopSoundGrabComponent();
}

void ACPP_PlayerPawnController::OnStopGrab() const
{
    if (PawnView)
    {
        PawnView->StopSoundGrabComponent();
        PawnModel->canListenToGrabSounds = true;
    }

    OnDroppedEvent.Broadcast();
}

void ACPP_PlayerPawnController::OnStopSFX() const
{
    if (PawnView)
    {
        PawnView->StopSoundSFXComponent();
        PawnModel->canListenToSounds = true;
    }
}

void ACPP_PlayerPawnController::FadeScreen(bool bOpenALevel) const
{
    if (PawnView)
    {
        PawnView->FadeToScenePawn(bOpenALevel);
    }
}

void ACPP_PlayerPawnController::BindOnGrabbed(UObject* Object, FName FunctionName)
{
    if (Object)
    {
        FScriptDelegate Delegate;
        Delegate.BindUFunction(Object, FunctionName);
        OnGrabbedEvent.Add(Delegate);
    }
}

void ACPP_PlayerPawnController::BindOnDropped(UObject* Object, FName FunctionName)
{
    if (Object)
    {
        FScriptDelegate Delegate;
        Delegate.BindUFunction(Object, FunctionName);
        OnDroppedEvent.Add(Delegate);
    }
}

void ACPP_PlayerPawnController::FadeOutSFXSound() const
{
    PawnView->FadeOutSoundSFXComponent();
}


EControllerHand ACPP_PlayerPawnController::GetHeldHand() const
{
    return PawnModel->HeldHand;
}

UCameraComponent* ACPP_PlayerPawnController::GetPlayerCameraComponent() const
{
    return Cast<UCameraComponent>(PawnView->GetComponentByClass(UCameraComponent::StaticClass()));
}
