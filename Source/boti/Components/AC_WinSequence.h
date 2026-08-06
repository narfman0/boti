#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_WinSequence.generated.h"

class ACameraActor;
class APostProcessVolume;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEscapeComplete);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BOTI_API UAC_WinSequence : public UActorComponent
{
	GENERATED_BODY()

public:
	UAC_WinSequence();

	// Assign in Blueprint/level — camera cut during escape
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WinSequence")
	TObjectPtr<ACameraActor> EscapeCamera;

	// Assign in Blueprint/level — post-process volume for fade to black
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WinSequence")
	TObjectPtr<APostProcessVolume> FadePostProcess;

	// Fired when the 2-second escape fade completes
	UPROPERTY(BlueprintAssignable, Category = "WinSequence")
	FOnEscapeComplete OnEscapeComplete;

	// Fades Anotsu out over 2s and plays a camera shake, then fires OnEscapeComplete
	UFUNCTION(BlueprintCallable, Category = "WinSequence")
	void PlayEscapeSequence(AActor* Anotsu);

	// Lerps FadePostProcess blend weight to 1 (black) over Duration seconds
	UFUNCTION(BlueprintCallable, Category = "WinSequence")
	void FadeToBlack(float Duration);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Escape fade state
	bool  bFadingAnotsu;
	float AnotsuFadeElapsed;
	float AnotsuFadeDuration;
	TWeakObjectPtr<AActor> FadingAnotsu;

	// FadeToBlack state
	bool  bFadingToBlack;
	float BlackFadeElapsed;
	float BlackFadeDuration;

	void FinishEscapeSequence();
};
