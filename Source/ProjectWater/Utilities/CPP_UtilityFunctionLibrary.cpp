// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_UtilityFunctionLibrary.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

void UCPP_UtilityFunctionLibrary::LogMessage(FString Message)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
}

void UCPP_UtilityFunctionLibrary::LogToFile(FString Message, FString FileName)
{
	FString FilePath = FPaths::ProjectLogDir() + FileName;
	FFileHelper::SaveStringToFile(Message + LINE_TERMINATOR, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
}

void UCPP_UtilityFunctionLibrary::LogOnScreen(FString Message, FLinearColor Color, float Duration)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color.ToFColor(true), Message);
	}
}

AActor* UCPP_UtilityFunctionLibrary::GetClosestActorOfClass(UObject* WorldContextObject, TSubclassOf<AActor> ActorClass,
	FVector Origin)
{
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	AActor* ClosestActor = nullptr;
	float ClosestDistance = FLT_MAX;

	for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
	{
		AActor* Actor = *It;
		float Distance = FVector::Dist(Origin, Actor->GetActorLocation());
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestActor = Actor;
		}
	}

	return ClosestActor;
}

bool UCPP_UtilityFunctionLibrary::HasLineOfSight(AActor* SourceActor, AActor* TargetActor)
{
	if (SourceActor && TargetActor)
	{
		FVector Start = SourceActor->GetActorLocation();
		FVector End = TargetActor->GetActorLocation();
		FHitResult HitResult;
		UWorld* World = SourceActor->GetWorld();
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(SourceActor);

		return World->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
	}
	return false;
}

