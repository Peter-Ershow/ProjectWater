#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPP_PlayerPawnController.generated.h"

class UCameraComponent;
class ACPP_PlayerPawnModel;
class ACPP_PlayerPawnView;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGrabbedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDroppedDelegate);

UCLASS(BlueprintType, Blueprintable)
class PROJECTWATER_API ACPP_PlayerPawnController : public APlayerController
{
	GENERATED_BODY()

public:
	// Constructor and input setup
	ACPP_PlayerPawnController();
	virtual void SetupInputComponent() override;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void VerifyAndPlaySFXComponent(USoundBase* InteractionSound, float Volume);

	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void VerifyAndPlayGrabComponent(USoundBase* InteractionSound, float Volume);

	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void StopSoundGrabComponent();

	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void OnGrab(EControllerHand NewHeldHand);
	
	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void OnStopGrab() const;

	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void OnStopSFX() const;

	UFUNCTION(BlueprintCallable, Category = "Screen Fade")
	void FadeScreen(bool bOpenALevel) const;
	
	FOnDroppedDelegate OnDroppedEvent;
	FOnGrabbedDelegate OnGrabbedEvent;

	void BindOnGrabbed(UObject* Object, FName FunctionName);
	void BindOnDropped(UObject* Object, FName FunctionName);

	void FadeOutSFXSound() const;

	EControllerHand GetHeldHand() const;

	UCameraComponent* GetPlayerCameraComponent() const;
	
private:
	UPROPERTY()
	ACPP_PlayerPawnView* PawnView;

	UPROPERTY()
	ACPP_PlayerPawnModel* PawnModel;
};
