#include "CPP_PuzzleItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectWater/Instances/CPP_GameInstance.h"
#include "ProjectWater/Actors/CPP_ActorToAssemble.h"
#include "Components/SceneComponent.h"
#include "ProjectWater/ProjectWater.h"
#include "ProjectWater/Items/CPP_AbstractItemActor.h"
#include "ProjectWater/Pawns/CPP_PlayerPawnController.h"

UCPP_PuzzleItemComponent::UCPP_PuzzleItemComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    GameInstance = Cast<UCPP_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("No UCPP_GameInstance found in PuzzleItemComponent."));
        return;
    }
    MainGroup = GameInstance->MainEGroup;
    Owner = Cast<AAbstractItemActor>(this);
    if(!Owner){
        UE_LOG(LogProjectWater, Error, TEXT(" No owner found in Puzzle Item Component"));
        return;
    }
    OnPuzzleSolved.AddDynamic(Owner, &AAbstractItemActor::HandlePuzzleSolved);
}

void UCPP_PuzzleItemComponent::BeginPlay()
{
    Super::BeginPlay();

    if(Owner->PlayerControllerRef)
    {
        Owner->PlayerControllerRef->BindOnGrabbed(this, "OnGrabbed");
        Owner->PlayerControllerRef->BindOnDropped(this, "OnDropped");
    }

    if (MainGroup == EGroup::Haptics || MainGroup == EGroup::Combined)
    {
        ACPP_ActorToAssemble* ActorToAssemble = Cast<ACPP_ActorToAssemble>(UGameplayStatics::GetActorOfClass(GetWorld(), ACPP_ActorToAssemble::StaticClass()));
        if (!ActorToAssemble)
        {
            UE_LOG(LogTemp, Warning, TEXT("Target Actor not found or invalid."));
            return;
        }
        bCorrectTag = FindCorrectTagInChildren(ActorToAssemble);
        ActorToAssemble->BindOnPuzzleSolved(this, "TriggerPuzzleSolved");
        
        if (bCorrectTag)
        {
            UE_LOG(LogTemp, Warning, TEXT("Correct tag found for: %s"), *PuzzleTag.ToString());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No correct tag found."));
        }
    }
}

void UCPP_PuzzleItemComponent::OnGrabbed()
{
    switch (MainGroup)
    {
    case EGroup::Visual:
        Owner->SetTextRenderVisibility(true);
        Owner->SetActorTickEnabled(true);
        break;
        
    case EGroup::Auditory:
        Owner->Interact();
        break;

    case EGroup::Haptics:
        if(!bCorrectTag)
        {
            Owner->PlayerControllerRef->PlayHapticEffect(WrongHapticEffect, Owner->PlayerControllerRef->GetHeldHand());
        }
        break;

    case EGroup::Control:
        break;
        
    case EGroup::Combined:
        Owner->SetTextRenderVisibility(true);
        Owner->SetActorTickEnabled(true);
        Owner->Interact();
        if(!bCorrectTag)
        {
            Owner->PlayerControllerRef->PlayHapticEffect(WrongHapticEffect, Owner->PlayerControllerRef->GetHeldHand());
        }
        break;
        
    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown EGroup value."));
        break;
    }
}

void UCPP_PuzzleItemComponent::OnDropped()
{
    //Try to resolve a puzzle if dropped into puzzle box container
    OnDroppedTryToResolvePuzzle.Broadcast();
    Owner->SetTextRenderVisibility(false);

    if(MainGroup==EGroup::Auditory || MainGroup == EGroup::Combined)
    {
        Owner->PlayerControllerRef->FadeOutSFXSound();
    }
}

bool UCPP_PuzzleItemComponent::FindCorrectTagInChildren(const AActor* ActorToAssemble) const
{
    const USceneComponent* RootComponent = Cast<USceneComponent>(ActorToAssemble->GetRootComponent());
    if (!RootComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("Root component of actor to assemble not found."));
        return false;
    }

    TArray<USceneComponent*> ChildComponents;
    RootComponent->GetChildrenComponents(true, ChildComponents);
    
    if (bPuzzleEnabled && PuzzleTag.IsValid())
    {
        for (const USceneComponent* ChildComponent : ChildComponents)
        {
            if (ChildComponent && ChildComponent->ComponentHasTag(PuzzleTag))
            {
                return true;
            }
        }
    }
    return false;
}

void UCPP_PuzzleItemComponent::TriggerPuzzleSolved()
{
    UE_LOG(LogTemp, Warning, TEXT("Triggering puzzle resolve in PuzzleItemComponent."));
    OnPuzzleSolved.Broadcast();
}
