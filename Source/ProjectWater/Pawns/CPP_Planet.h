// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Planet.generated.h"

UCLASS()
class PROJECTWATER_API ACPP_Planet : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Planet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Components
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	AActor* TargetLocation;

	// Function to check if the planet has reached the target
	void CheckIfReachedTarget();
};
