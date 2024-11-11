// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CPP_UtilityFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTWATER_API UCPP_UtilityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Logs a message to the Output Log and optionally prints on screen
	UFUNCTION(BlueprintCallable, Category = "Debug")
	static void LogMessage(FString Message);

	// Logs a message to a specified file
	UFUNCTION(BlueprintCallable, Category = "Debug")
	static void LogToFile(FString Message, FString FileName = "GameLog.txt");

	// Logs a message to the screen with a specified color and duration
	UFUNCTION(BlueprintCallable, Category = "Debug")
	static void LogOnScreen(FString Message, FLinearColor Color = FLinearColor::White, float Duration = 5.0f);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static AActor* GetClosestActorOfClass(UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, FVector Origin);
	
	// Check line of sight between two actors
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool HasLineOfSight(AActor* SourceActor, AActor* TargetActor);
};
