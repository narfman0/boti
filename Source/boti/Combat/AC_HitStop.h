#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_HitStop.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOTI_API UAC_HitStop : public UActorComponent
{
	GENERATED_BODY()

public:
	UAC_HitStop();

	// 0.05s at 0.05x dilation on parry/riposte.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitStop")
	float ParryHitStopDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitStop")
	float ParryTimeDilation;

	// 0.03s at 0.1x on normal hits.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitStop")
	float NormalHitStopDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitStop")
	float NormalTimeDilation;

	UFUNCTION(BlueprintCallable, Category = "HitStop")
	void ApplyHitStop(float Duration, float TimeDilation);

	UFUNCTION(BlueprintCallable, Category = "HitStop")
	void ApplyParryHitStop();

	UFUNCTION(BlueprintCallable, Category = "HitStop")
	void ApplyNormalHitStop();

private:
	FTimerHandle HitStopTimer;

	void RestoreTimeDilation();
};
