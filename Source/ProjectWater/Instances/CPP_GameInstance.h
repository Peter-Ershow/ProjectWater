// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectWater/Enums/EGroup.h"
#include "CPP_GameInstance.generated.h"
/**
 * 
 */
UCLASS()
class PROJECTWATER_API UCPP_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	UPROPERTY(BlueprintReadOnly)
	EGroup MainEGroup;

	UFUNCTION(BlueprintCallable, Category= "Puzzle")
	void SetCurrentState(EGroup NewState);
	
	UPROPERTY(BlueprintReadWrite, Category= "Miscellaneous")
	FDateTime StartTime;

	bool bPuzzleSolved = false;
};