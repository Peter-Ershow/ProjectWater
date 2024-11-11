// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_ActorToAssemble.h"

#include "Components/TimelineComponent.h"
#include "ProjectWater/Instances/CPP_GameInstance.h"

// Sets default values
ACPP_ActorToAssemble::ACPP_ActorToAssemble()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ScaleTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ScaleTimeline"));
	TargetComponent = nullptr;

}

// Called when the game starts or when spawned
void ACPP_ActorToAssemble::BeginPlay()
{
	Super::BeginPlay();

	TArray<USceneComponent*> ChildrenComponents;
	RootComponent->GetChildrenComponents(true, ChildrenComponents);

	for (USceneComponent* Child : ChildrenComponents)
	{
		if (UStaticMeshComponent* StaticMeshComp = Cast<UStaticMeshComponent>(Child))
		{
			StaticMeshComp->SetWorldScale3D(FVector(0.0f, 0.0f, 0.0f));
		}
	}
	
	InitializeTimeline();
}

void ACPP_ActorToAssemble::UpdateScale(float ScaleValue)
{
	if (TargetComponent)
	{
		FVector NewScale = FVector(ScaleValue, ScaleValue, ScaleValue);
		TargetComponent->SetWorldScale3D(NewScale);
	}
}

void ACPP_ActorToAssemble::OnTimelineFinished()
{
	bool ConditionMet = false;

	TArray<USceneComponent*> ChildComponents;
	GetRootComponent()->GetChildrenComponents(true, ChildComponents);

	for (USceneComponent* Child : ChildComponents)
	{
		if (UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(Child))
		{
			if (MeshComponent->GetComponentScale().IsNearlyZero())
				{
				ConditionMet = false;
				break;
				}
		}
	}
	
	if (ConditionMet)
	{
		UE_LOG(LogTemp, Log, TEXT("Puzzle Solved!"));
		GameInstanceRef->bPuzzleSolved = true;
		OnPuzzleSolved.Broadcast();
	}
}

// Called every frame
void ACPP_ActorToAssemble::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPP_ActorToAssemble::BindOnPuzzleSolved(UObject* Object, FName FunctionName)
{
	if (Object)
	{
		FScriptDelegate Delegate;
		Delegate.BindUFunction(Object, FunctionName);
		OnPuzzleSolved.Add(Delegate);
	}
}

void ACPP_ActorToAssemble::SuccessEvent(FName Tag)
{
	TargetComponent = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if (TargetComponent && TargetComponent->ComponentHasTag(Tag)) 
		{
		UE_LOG(LogTemp, Log, TEXT("Found Target Component with the specified tag."));
		}
	
}

void ACPP_ActorToAssemble::InitializeTimeline()
{
	if (ScaleTimeline && ScaleCurve)
	{
		FOnTimelineFloat TimelineCallback;
		TimelineCallback.BindUFunction(this, FName("UpdateScale"));
		ScaleTimeline->AddInterpFloat(ScaleCurve, TimelineCallback); 

		FOnTimelineEventStatic TimelineFinishedCallback;
		TimelineFinishedCallback.BindUFunction(this, FName("OnTimelineFinished"));
		ScaleTimeline->SetTimelineFinishedFunc(TimelineFinishedCallback);
	}
}
