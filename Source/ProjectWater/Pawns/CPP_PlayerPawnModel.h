#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CPP_PlayerPawnModel.generated.h"

UCLASS(BlueprintType, Blueprintable)
class PROJECTWATER_API ACPP_PlayerPawnModel : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Data Export")
	static void ExportDataToCsv(const FString& FilePrefix, const FString& Header, const FString& DataRow);

	UFUNCTION(BlueprintCallable, Category = "Data Export")
	void ExportLookDataToCsv(FString ObjectName, float TimeLookedAt) const;

	UFUNCTION(BlueprintCallable, Category = "Data Export")
	void ExportGrabDataToCsv(FString ObjectName) const;

	UFUNCTION(BlueprintCallable, Category = "Data Export")
	void ExportLevelDataToCsv(FString StepName) const;

	// Flags for sound control
	UPROPERTY(BlueprintReadWrite, Category = "Sound Control")
	bool canListenToGrabSounds = false;

	UPROPERTY(BlueprintReadWrite, Category = "Sound Control")
	bool canListenToSounds = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab")
	EControllerHand HeldHand;

private:
	// Helper function to get elapsed time
	UFUNCTION(BlueprintPure, Category = "Utility")
	float GetElapsedTime() const;
};
