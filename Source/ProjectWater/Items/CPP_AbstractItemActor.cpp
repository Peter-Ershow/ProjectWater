// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AbstractItemActor.h"

#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectWater/ProjectWater.h"
#include "ProjectWater/Pawns/CPP_PlayerPawnController.h"

AAbstractItemActor::AAbstractItemActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	SetRootComponent(StaticMeshComponent);
	
	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	TextRender->SetHorizontalAlignment(EHTA_Center);
	TextRender->SetText(FText::FromString("Puzzle Text"));
	TextRender->SetTextRenderColor(FColor::White);
	TextRender->SetWorldSize(30.0f);
	TextRender->SetupAttachment(RootComponent);

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComp->SetupAttachment(RootComponent);
	NiagaraComp->bAutoActivate = false;
}

void AAbstractItemActor::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = Cast<ACPP_PlayerPawnController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if(!PlayerControllerRef)
	{
		UE_LOG(LogProjectWater, Warning, TEXT("[%s::%s] PlayerController not found."),
			   *GetClass()->GetName(), TEXT(__FUNCTION__));

		PlayerCameraComponent = PlayerControllerRef->GetPlayerCameraComponent();
	}
}

void AAbstractItemActor::SetTextRenderVisibility(bool bVisible) const
{
	TextRender->SetVisibility(bVisible);
}

void AAbstractItemActor::HandlePuzzleSolved()
{
	UE_LOG(LogTemp, Log, TEXT("Puzzle Solved! Handling logic here."));
	ToggleMeshVisibility(StaticMeshComponent, false); 
}

void AAbstractItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//When Tick enabled, render the text render and rotate towards player's camera
	if(TextRender->IsVisible())
	{
		if(PlayerCameraComponent)
		{
			TextRender->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(TextRender->GetComponentLocation(), PlayerCameraComponent->GetComponentLocation()));
		}
	}
}

void AAbstractItemActor::Interact_Implementation()
{
	if (InteractionSound)
	{
		UE_LOG(LogProjectWater, Warning, TEXT("Trying to trigger playing sfx sound: %s"), *InteractionSound->GetName());
		PlayerControllerRef->VerifyAndPlaySFXComponent(InteractionSound, 1);
	}
}

void AAbstractItemActor::TriggerDisappear()
{
	NiagaraComp->Activate();
	NiagaraComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	StaticMeshComponent->SetVisibility(false);
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AAbstractItemActor::DestroySelf, 2.0f, false);
}

void AAbstractItemActor::DestroySelf()
{
	Destroy();
}

void AAbstractItemActor::ToggleMeshVisibility(UStaticMeshComponent* Mesh, bool bIsVisible)
{
	if (Mesh)
	{
		Mesh->SetVisibility(bIsVisible, true);
	}
}


