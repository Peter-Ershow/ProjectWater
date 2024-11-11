#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPP_PuzzleItemComponent.generated.h"

class UGameplayStatics;
enum class EGroup : uint8;
class AAbstractItemActor;
class UTextRenderComponent;
class ACPP_ActorToAssemble;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTWATER_API UCPP_PuzzleItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCPP_PuzzleItemComponent();

protected:
	virtual void BeginPlay() override;

public:
	void TriggerPuzzleSolved();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPuzzleSolved);
	UPROPERTY(BlueprintAssignable, Category = "Puzzle Events")
	FOnPuzzleSolved OnPuzzleSolved;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDroppedTryToResolvePuzzle);
	FOnDroppedTryToResolvePuzzle OnDroppedTryToResolvePuzzle;

	UFUNCTION()
	void OnGrabbed();
	
	UFUNCTION()
	void OnDropped();

	UPROPERTY(EditAnywhere, Category = "Haptics")
	UHapticFeedbackEffect_Base* WrongHapticEffect;

private:
	UPROPERTY()
	bool bCorrectTag;

	UPROPERTY(EditAnywhere, Category = "Puzzle")
	FName PuzzleTag;

	UPROPERTY(EditAnywhere, Category = "Puzzle")
	bool bPuzzleEnabled;

	UPROPERTY()
	class UCPP_GameInstance* GameInstance;

	// Helper function to find the actor to assemble and check child components for tags
	bool FindCorrectTagInChildren(const AActor* ActorToAssemble) const;

	UPROPERTY()
	AAbstractItemActor* Owner;

	EGroup MainGroup;
};
