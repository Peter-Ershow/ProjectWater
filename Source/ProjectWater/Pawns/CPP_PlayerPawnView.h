#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CPP_PlayerPawnView.generated.h"

class ACPP_PlayerPawnController;
class UAudioComponent;
class USoundBase;
class UTimelineComponent;
class ACPP_Screenfade;

UCLASS(BlueprintType, Blueprintable)
class PROJECTWATER_API ACPP_PlayerPawnView : public APawn
{
    GENERATED_BODY()

public:
    ACPP_PlayerPawnView();

    // Audio playback functions
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlaySoundGrabComponent(USoundBase* InteractionSound, float Volume);

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlaySoundSFXComponent(USoundBase* InteractionSound, float Volume);

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void StopSoundGrabComponent();

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void StopSoundSFXComponent();

    void FadeOutSoundSFXComponent();

    // Screen fade management
    UFUNCTION(BlueprintCallable, Category = "Screen Fade")
    void FadeToScenePawn(bool bOpenALevel);

    UFUNCTION(BlueprintCallable, Category = "Screen Fade")
    void UpdateScreenFade(float Value, UMaterialInstanceDynamic* DynamicMaterial);

    UFUNCTION(BlueprintCallable, Category = "Screen Fade")
    void OnFadeFinished();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Fade")
    UCurveFloat* FadeCurve;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Fade")
    UTimelineComponent* ScreenFadeTimeline;

protected:
    virtual void BeginPlay() override;

private:
    ACPP_PlayerPawnController* PlayerController;

    
    void InitializeAudioComponents();
    void InitializeScreenFadeComponents();

    UFUNCTION()
    void OnGrabSoundFinished();

    UFUNCTION()
    void OnSFXSoundFinished();

    UFUNCTION()
    void OnMusicSoundFinished();

    UFUNCTION()
    void DestroyAfterDelay();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
    UAudioComponent* MusicAudioComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
    UAudioComponent* SFXAudioComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
    UAudioComponent* GrabAudioComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Fade", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<ACPP_Screenfade> ScreenFadeActor;

    UPROPERTY(BlueprintReadOnly, Category = "Screen Fade", meta = (AllowPrivateAccess = "true"))
    ACPP_Screenfade* SpawnedActor;

    FTimerHandle ScreenFadeTimerHandle;
};
