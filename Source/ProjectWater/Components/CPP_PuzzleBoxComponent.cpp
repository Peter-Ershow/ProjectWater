// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PuzzleBoxComponent.h"

#include "CPP_PuzzleItemComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectWater/Interfaces/Interactable.h"
#include "..\Items\CPP_AbstractItemActor.h"
#include "ProjectWater/Pawns/CPP_PlayerPawn.h"

UCPP_PuzzleBoxComponent::UCPP_PuzzleBoxComponent()
{
	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComp->SetupAttachment(this);
	NiagaraComp->bAutoActivate = false;

	OnComponentBeginOverlap.AddDynamic(this, &UCPP_PuzzleBoxComponent::OnOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UCPP_PuzzleBoxComponent::OnOverlapEnd);
}
void UCPP_PuzzleBoxComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
	if (BorderEffect && NiagaraComp)
	{
		NiagaraComp->SetAsset(BorderEffect);
		NiagaraComp->Activate();
	}
}

void UCPP_PuzzleBoxComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!BoxSolved)
	{
		UE_LOG(LogTemp, Warning, TEXT(" Some Overlap begin"));
		if (GetOwner() && OtherActor->Implements<UInteractable>())
		{
			if(!OtherActor->Tags.IsEmpty())
			{
				if(AAbstractItemActor* ItemActor = Cast<AAbstractItemActor>(OtherActor))
				{
					if(!ItemActor->FindComponentByClass<UCPP_PuzzleItemComponent>()->OnDroppedTryToResolvePuzzle.IsAlreadyBound(this, &UCPP_PuzzleBoxComponent::TriggerPuzzleResolve))
					{
						ItemActor->FindComponentByClass<UCPP_PuzzleItemComponent>()->OnDroppedTryToResolvePuzzle.AddDynamic(this, &UCPP_PuzzleBoxComponent::TriggerPuzzleResolve);
					}
				}
			}
		}
	}
}

void UCPP_PuzzleBoxComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(AAbstractItemActor* ItemActor = Cast<AAbstractItemActor>(OtherActor))
	{
		ItemActor->FindComponentByClass<UCPP_PuzzleItemComponent>()->OnDroppedTryToResolvePuzzle.RemoveDynamic(this, &UCPP_PuzzleBoxComponent::TriggerPuzzleResolve);
	}
}

void UCPP_PuzzleBoxComponent::TriggerPuzzleResolve()
{

	TArray<AActor*> OverlappingActors;
	this->GetOverlappingActors(OverlappingActors, AAbstractItemActor::StaticClass());
	
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && IsValid(Actor))
		{
			FName ItemTag = Actor->Tags[0];
			//correct resolve
			if (ItemTag != NAME_None && ItemTag == CorrectTag)
			{
				UE_LOG(LogTemp, Warning, TEXT("Overlapping Actor: %s, Class: %s"), *Actor->GetName(), *Actor->GetClass()->GetName());
				
				// Disable physics and collisions
				UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
				if (PrimitiveComp)
				{
					PrimitiveComp->SetSimulatePhysics(false);
					PrimitiveComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					Actor->SetActorEnableCollision(false);
					
					if(AAbstractItemActor* AbstractItemActor = Cast<AAbstractItemActor>(Actor))
					{
						AbstractItemActor->TriggerDisappear();
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("No Primitive Component Found"));
				}
				
				SetNiagaraColor(FLinearColor::Green);
				
				if (CorrectOverlapSound) {
					UGameplayStatics::PlaySoundAtLocation(this, CorrectOverlapSound, GetComponentLocation());
				}
				
				BoxSolved = true;

				ExportToStepsCsv("Solved " + CorrectTag.ToString());
				
				OnBoxOverlap.Broadcast(CorrectTag);
			}
			//Incorrect resolve
			else
			{
				SetNiagaraColor(FLinearColor::Red);
				GetWorld()->GetTimerManager().SetTimer(ColorResetTimerHandle, this, &UCPP_PuzzleBoxComponent::ResetColor, 5.0f, false);
				
				// Play a sound at the box component's location
				if (WrongOverlapSound) // OverlapSound should be a USoundBase* or USoundCue* property
				{
				UGameplayStatics::PlaySoundAtLocation(this, WrongOverlapSound, GetComponentLocation());
				}

				ExportToStepsCsv("Wronged " + CorrectTag.ToString() + " with " + Actor->Tags[0].ToString());
			}
		}
	}
}

void UCPP_PuzzleBoxComponent::ResetColor()
{
	SetNiagaraColor(FLinearColor::Blue);

	// Optionally, clear the timer if you want to make sure it's not active anymore
	GetWorld()->GetTimerManager().ClearTimer(ColorResetTimerHandle);
}

void UCPP_PuzzleBoxComponent::SetNiagaraColor(FLinearColor Color)
{
	if (!NiagaraComp) return;
	
	NiagaraComp->SetVariableLinearColor(FName("BorderColor"), Color);
}

void UCPP_PuzzleBoxComponent::ExportToStepsCsv(FString Text)
{
	if(ACPP_PlayerPawn* PlayerPawn = Cast<ACPP_PlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		PlayerPawn->ExportLevelDataToCsv(Text);
	}
}
