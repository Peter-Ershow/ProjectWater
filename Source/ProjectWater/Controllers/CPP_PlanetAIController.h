// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CPP_PlanetAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTWATER_API ACPP_PlanetAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
