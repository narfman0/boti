#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_PostProcess.generated.h"

class UPostProcessComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOTI_API UAC_PostProcess : public UActorComponent
{
	GENERATED_BODY()

public:
	UAC_PostProcess();

	// Seconds to lerp vignette in/out when entering/leaving combat.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostProcess")
	float VignetteLerpTime;

	// Target vignette intensity when combat is active.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostProcess")
	float CombatVignetteIntensity;

	// Target desaturation + red-tint intensity when downed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostProcess")
	float DownedColorGradingIntensity;

	UFUNCTION(BlueprintCallable, Category = "PostProcess")
	void SetCombatVignette(bool bActive);

	UFUNCTION(BlueprintCallable, Category = "PostProcess")
	void SetDownedEffect(bool bActive);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UPostProcessComponent* PPComp;

	float CurrentVignette;
	float TargetVignette;

	float CurrentColorGrading;
	float TargetColorGrading;

	void FindOrCreatePPComponent();
	void ApplySettings();
};
