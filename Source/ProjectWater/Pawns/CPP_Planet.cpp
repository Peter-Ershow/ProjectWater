// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Planet.h"

#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACPP_Planet::ACPP_Planet(): TargetLocation(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize movement parameters
	MovementSpeed = 100000000.0f;
}

// Called when the game starts or when spawned
void ACPP_Planet::BeginPlay()
{
	Super::BeginPlay();
	// Find the first TargetPoint in the level
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		TargetLocation = FoundActors[0];
	}
}

// Called every frame
void ACPP_Planet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move towards the target location
	FVector CurrentLocation = GetActorLocation();
	FVector Direction = (TargetLocation->GetActorLocation() - CurrentLocation).GetSafeNormal();
	FVector NewLocation = CurrentLocation + (Direction * MovementSpeed * DeltaTime);
	SetActorLocation(NewLocation);

	// Check if the planet has reached the target
	CheckIfReachedTarget();

}

void ACPP_Planet::CheckIfReachedTarget()
{
	FVector CurrentLocation = GetActorLocation();
	float DistanceToTarget = FVector::Dist(CurrentLocation, TargetLocation->GetActorLocation());

	if (DistanceToTarget <= 10.0f) // Adjust tolerance as needed
		{
		// End the level
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
		}
}

