// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_ActorToAssemble.generated.h"

class UCPP_GameInstance;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPuzzleSolvedDelegate);

UCLASS()
class PROJECTWATER_API ACPP_ActorToAssemble : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_ActorToAssemble();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Callback for timeline updates
	UFUNCTION()
	void UpdateScale(float ScaleValue);

	// Callback when timeline finishes
	UFUNCTION()
	void OnTimelineFinished();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	class UTimelineComponent* ScaleTimeline;

	UPROPERTY()
	UStaticMeshComponent* TargetComponent;

	FOnPuzzleSolvedDelegate OnPuzzleSolved;

	void BindOnPuzzleSolved(UObject* Object, FName FunctionName);
	
	UFUNCTION()
	void SuccessEvent(FName Tag);

	UPROPERTY()
	UCPP_GameInstance* GameInstanceRef;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UCurveFloat* ScaleCurve;
	
	// Function to initialize the timeline
	void InitializeTimeline();
	
};
