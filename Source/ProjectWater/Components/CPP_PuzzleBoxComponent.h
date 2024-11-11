// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "CPP_PuzzleBoxComponent.generated.h"

class UTextRenderComponent;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBoxOverlapSignature, FName, BoxTag);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTWATER_API UCPP_PuzzleBoxComponent : public UBoxComponent
{
	GENERATED_BODY()
	
public:
	UCPP_PuzzleBoxComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Puzzle")
	FName CorrectTag;

	// Event that gets broadcast when this component is overlapped
	UPROPERTY(BlueprintAssignable, Category="Puzzle")
	FOnBoxOverlapSignature OnBoxOverlap;

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	UNiagaraComponent* NiagaraComp;

	// Niagara system to be used for the border effect
	UPROPERTY(EditAnywhere, Category="Effects")
	UNiagaraSystem* BorderEffect;

	UPROPERTY(EditAnywhere, Category="Effects")
	USoundBase* WrongOverlapSound; // Set this in the editor or via code

	UPROPERTY(EditAnywhere, Category="Effects")
	USoundBase* CorrectOverlapSound; // Set this in the editor or via code
	
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void TriggerPuzzleResolve();
	
	// Function to change the color back to blue
	void ResetColor();
	
	void ExportToStepsCsv(FString Text);

	bool BoxSolved = false;

	// Timer handle to manage the reset
	FTimerHandle ColorResetTimerHandle;

	void SetNiagaraColor(FLinearColor Color);
};



