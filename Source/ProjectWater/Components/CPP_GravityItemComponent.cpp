#include "CPP_GravityItemComponent.h"

#include "Components/TextRenderComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "ProjectWater/ProjectWater.h"
#include "ProjectWater/Enums/EGroup.h"
#include "ProjectWater/Instances/CPP_GameInstance.h"
#include "ProjectWater/Items/CPP_AbstractItemActor.h"
#include "ProjectWater/Pawns/CPP_PlayerPawnController.h"

UCPP_GravityItemComponent::UCPP_GravityItemComponent()
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
    Owner->StaticMeshComponent->OnComponentHit.AddDynamic(this, &UCPP_GravityItemComponent::OnGroundHit);

    FallEffectTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("FallEffectTimeline"));

}

void UCPP_GravityItemComponent::BeginPlay()
{
    Super::BeginPlay();

    if(Owner->PlayerControllerRef)
    {
        Owner->PlayerControllerRef->BindOnGrabbed(this, "OnGrabbed");
        Owner->PlayerControllerRef->BindOnDropped(this, "OnDropped");
    }

    Gravity = GetWorld()->GetWorldSettings()->GlobalGravityZ;

    Owner->TextRender->SetText(FText::AsNumber(Weight));

    SetComponentTickEnabled(false);
    
    if (ThicknessCurve)
    {
        SetupFallingTimeline();
    }

    if(MPCCollectionAsset)
    {
        MPCCollection = GetWorld()->GetParameterCollectionInstance(MPCCollectionAsset);
    }

    Weight = Owner->StaticMeshComponent->GetMass();
}

void UCPP_GravityItemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    Intensity = CalculateIntensity(Owner->StaticMeshComponent->GetPhysicsLinearVelocity().Length());

    switch (MainGroup)
    {
    case EGroup::Visual:
        MPCCollection->SetScalarParameterValue(FName("Thickness"), Intensity * 2.0f);
        break;
        
    case EGroup::Auditory:
        if(MovingSound != nullptr)
        {
            Owner->PlayerControllerRef->VerifyAndPlayGrabComponent(MovingSound, Intensity * 2.0f);
        }
        break;

    case EGroup::Haptics:

        if(HapticEffect)
        {
            Owner->PlayerControllerRef->PlayHapticEffect(HapticEffect, Owner->PlayerControllerRef->GetHeldHand(), Intensity);
        }
        break;

    case EGroup::Control:
        break;
        
    case EGroup::Combined:
        MPCCollection->SetScalarParameterValue(FName("Thickness"), Intensity * 2.0f);
        if(MovingSound != nullptr)
        {
            Owner->PlayerControllerRef->VerifyAndPlayGrabComponent(MovingSound, Intensity * 2.0f);
        }
        if(HapticEffect)
        {
            Owner->PlayerControllerRef->PlayHapticEffect(HapticEffect, Owner->PlayerControllerRef->GetHeldHand(), Intensity);
        }
        break;
        
    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown EGroup value."));
        break;
    }
}

void UCPP_GravityItemComponent::OnGrabbed()
{
    SetComponentTickEnabled(true);
    bFallingSoundPlayed = false;
    if(MainGroup == EGroup::Combined || MainGroup == EGroup::Visual)
    {
        Owner->SetTextRenderVisibility(true);
    }
}

void UCPP_GravityItemComponent::OnDropped()
{
    SetComponentTickEnabled(false);
    Owner->PlayerControllerRef->StopSoundGrabComponent();
    MPCCollection->SetScalarParameterValue(FName("Thickness"),0.0f);
    Owner->SetTextRenderVisibility(false);
}

float UCPP_GravityItemComponent::CalculateIntensity(const float GrabSpeed) const
{
    const float IntensityUnclamped = FMath::Abs((GrabSpeed * Gravity * Weight) / 980.0f) / 75000.0f;
    
    return FMath::Clamp(IntensityUnclamped, 0.0f, 1.0f);
}

void UCPP_GravityItemComponent::OnGroundHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!HitComponent || !OtherActor)
    {
        return;
    }
    
    if (HitComponent->ComponentHasTag("Ground") || OtherActor->ActorHasTag("Ground"))
    {
        switch (MainGroup)
        {
        case EGroup::Visual:
            FallEffectTimeline->PlayFromStart();
            break;
        
        case EGroup::Auditory:
            if(!bFallingSoundPlayed)
            {
                if(FallingSound)
                {
                    Owner->PlayerControllerRef->VerifyAndPlayGrabComponent(FallingSound, Intensity);
                    bFallingSoundPlayed = true;
                }
            }
            break;

        case EGroup::Haptics:
            if(HapticEffect)
            {
                Owner->PlayerControllerRef->PlayHapticEffect(HapticEffect, Owner->PlayerControllerRef->GetHeldHand(), Intensity);
            }
            break;

        case EGroup::Control:
            break;
        
        case EGroup::Combined:
            FallEffectTimeline->PlayFromStart();
            if(!bFallingSoundPlayed)
            {
                if(FallingSound)
                {
                    Owner->PlayerControllerRef->VerifyAndPlayGrabComponent(FallingSound, Intensity);
                    bFallingSoundPlayed = true;
                }
            }
            if(HapticEffect)
            {
                Owner->PlayerControllerRef->PlayHapticEffect(HapticEffect, Owner->PlayerControllerRef->GetHeldHand(), Intensity);
            }
            break;
        
        default:
            UE_LOG(LogTemp, Warning, TEXT("Unknown EGroup value."));
            break;
        }
    }
}

void UCPP_GravityItemComponent::UpdateThickness(float Value) const
{
    if (MPCCollection)
    {
        if (MPCCollection)
        {
            MPCCollection->SetScalarParameterValue(FName("Thickness"), Value * Intensity);
        }
    }
}

void UCPP_GravityItemComponent::SetupFallingTimeline()
{
    FOnTimelineFloat UpdateFunction;
    UpdateFunction.BindUFunction(this, FName("UpdateThickness"));
    FallEffectTimeline->AddInterpFloat(ThicknessCurve, UpdateFunction);
}
