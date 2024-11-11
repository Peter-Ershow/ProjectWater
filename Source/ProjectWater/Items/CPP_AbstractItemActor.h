#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectWater/Interfaces/Interactable.h"
#include "CPP_AbstractItemActor.generated.h"

class UNiagaraComponent;
class ACPP_PlayerPawnController;
class UTextRenderComponent;
class UCPP_PuzzleItemComponent;
class UCPP_GameInstance;
enum class EGroup : uint8;
class UCameraComponent;
enum class EControllerType : uint8;
class ACPP_PlayerPawn;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPuzzleSolved);

UCLASS(Abstract, BlueprintType, Blueprintable)
class PROJECTWATER_API AAbstractItemActor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbstractItemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY()
	ACPP_PlayerPawnController* PlayerControllerRef;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void Interact_Implementation() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects Assignees")
	USoundBase* InteractionSound;
	
	void TriggerDisappear();
	void DestroySelf();

	UPROPERTY()
	UNiagaraComponent* NiagaraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Puzzle")
	FName PuzzleTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle")
	UTextRenderComponent* TextRender;

	void SetTextRenderVisibility(bool bVisible) const;

	void HandlePuzzleSolved();
private:
	static void ToggleMeshVisibility(UStaticMeshComponent* Mesh, bool bIsVisible);

	UPROPERTY()
	UCPP_GameInstance* GameInstance;

	FTimerHandle DestroyTimerHandle;

	UPROPERTY()
	UCameraComponent* PlayerCameraComponent;
};


