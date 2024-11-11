// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_ActorToAssemble.h"
#include "GameFramework/Actor.h"
#include "CPP_PuzzleManager.generated.h"

UCLASS()
class PROJECTWATER_API ACPP_PuzzleManager : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	ACPP_PuzzleManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ACPP_ActorToAssemble* SolutionActor;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	FName PuzzleTag;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleSuccessPuzzleEvent(FName BoxTag);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPuzzleSolvedDelegate, FName, BoxTag);
	FPuzzleSolvedDelegate OnPuzzleSolved;
	
  };
