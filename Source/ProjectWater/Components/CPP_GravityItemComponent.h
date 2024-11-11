#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPP_GravityItemComponent.generated.h"

class UTimelineComponent;
class AAbstractItemActor;
enum class EGroup : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTWATER_API UCPP_GravityItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCPP_GravityItemComponent();
protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	UFUNCTION()
	void OnGrabbed();
	
	UFUNCTION()
	void OnDropped();

	UPROPERTY(EditAnywhere, Category = "Haptics")
	UHapticFeedbackEffect_Base* WrongHapticEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Effects Assignees")
	UHapticFeedbackEffect_Base* HapticEffect;

private:
	UPROPERTY()
	class UCPP_GameInstance* GameInstance;
	
	UPROPERTY()
	AAbstractItemActor* Owner;

	EGroup MainGroup;
	
	float Gravity;

	float Weight;

	UPROPERTY()
	USoundBase* FallingSound;

	UPROPERTY()
	USoundBase* MovingSound;

	bool bFallingSoundPlayed;

	UPROPERTY()
	UMaterialParameterCollectionInstance* MPCCollection;

	UPROPERTY(EditAnywhere)
	UMaterialParameterCollection* MPCCollectionAsset;

	float CalculateIntensity(float GrabSpeed) const;
	
	UFUNCTION()
	void OnGroundHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY()
	UTimelineComponent* FallEffectTimeline;

	UFUNCTION()
	void UpdateThickness(float Value) const;

	void SetupFallingTimeline();
	
	UPROPERTY(EditAnywhere)
	UCurveFloat* ThicknessCurve;
	
	float Intensity;
};