#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_Posture.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	Normal       UMETA(DisplayName = "Normal"),
	Staggered    UMETA(DisplayName = "Staggered"),
	PostureBreak UMETA(DisplayName = "PostureBreak"),
	Executing    UMETA(DisplayName = "Executing"),
};

UENUM(BlueprintType)
enum class EHitMagnitude : uint8
{
	Light  UMETA(DisplayName = "Light"),
	Heavy  UMETA(DisplayName = "Heavy"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPostureBreak);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPostureRestored);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitReaction, EHitMagnitude, Magnitude);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOTI_API UAC_Posture : public UActorComponent
{
	GENERATED_BODY()

public:
	UAC_Posture();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Posture")
	float PostureMax;

	UPROPERTY(BlueprintReadOnly, Category = "Posture")
	float PostureCurrent;

	UPROPERTY(BlueprintReadOnly, Category = "Posture")
	ECombatState CombatState;

	// Seconds of no posture damage before regen begins
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Posture")
	float RegenDelay;

	// Posture restored per second during regen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Posture")
	float RegenRate;

	// Duration of stagger state after parry success
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Posture")
	float StaggerDuration;

	// Duration of the posture-break execution window
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Posture")
	float PostureBreakDuration;

	// Posture damage above this fraction of PostureMax counts as a heavy hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Posture")
	float HeavyHitThreshold;

	UPROPERTY(BlueprintAssignable, Category = "Posture")
	FOnPostureBreak OnPostureBreak;

	UPROPERTY(BlueprintAssignable, Category = "Posture")
	FOnPostureRestored OnPostureRestored;

	UPROPERTY(BlueprintAssignable, Category = "Posture")
	FOnHitReaction OnHitReaction;

	// Apply posture damage. Returns true if posture broke this hit.
	UFUNCTION(BlueprintCallable, Category = "Posture")
	bool ApplyPostureDamage(float Damage);

	// Force stagger — called on parry success against this actor.
	UFUNCTION(BlueprintCallable, Category = "Posture")
	void EnterStagger();

	UFUNCTION(BlueprintCallable, Category = "Posture")
	bool IsPostureBroken() const { return CombatState == ECombatState::PostureBreak; }

	UFUNCTION(BlueprintCallable, Category = "Posture")
	bool IsStaggered() const { return CombatState == ECombatState::Staggered; }

	UFUNCTION(BlueprintCallable, Category = "Posture")
	float GetPosturePercent() const { return PostureMax > 0.f ? PostureCurrent / PostureMax : 0.f; }

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	float TimeSinceLastPostureDamage;
	FTimerHandle StaggerTimer;
	FTimerHandle PostureBreakTimer;

	void BreakPosture();
	void EndStagger();
	void EndPostureBreak();
};
