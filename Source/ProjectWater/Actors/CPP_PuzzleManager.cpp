// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PuzzleManager.h"

#include "ProjectWater/Components/CPP_PuzzleBoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACPP_PuzzleManager::ACPP_PuzzleManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPP_ActorToAssemble::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if(PuzzleTag.IsValid())
		{
			if(Actor->ActorHasTag(PuzzleTag))
			{
				UE_LOG(LogTemp, Warning, TEXT(" Found Correct Solution Actor"));
				SolutionActor = Cast<ACPP_ActorToAssemble>(Actor);
			}
		}
	}
}

// Called when the game starts or when spawned
void ACPP_PuzzleManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<USceneComponent*> ChildrenComponents;
	RootComponent->GetChildrenComponents(true, ChildrenComponents);

	for (auto ChildrenComponent : ChildrenComponents)
	{
		if(UCPP_PuzzleBoxComponent* PuzzleComponent = Cast<UCPP_PuzzleBoxComponent>(ChildrenComponent))
		{
			PuzzleComponent->OnBoxOverlap.AddDynamic(this, &ACPP_PuzzleManager::HandleSuccessPuzzleEvent);
		}
	}
}

// Called every frame
void ACPP_PuzzleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_PuzzleManager::HandleSuccessPuzzleEvent(FName BoxTag)
{
	if(BoxTag != NAME_None && SolutionActor)
	{
		OnPuzzleSolved.Broadcast(BoxTag);
		SolutionActor-> SuccessEvent(BoxTag);
	}
}

