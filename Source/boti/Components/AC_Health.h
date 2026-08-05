#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_Health.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDowned);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRevived);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOTI_API UAC_Health : public UActorComponent
{
	GENERATED_BODY()

public:
	UAC_Health();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HPMax;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float HPCurrent;

	// Seconds in the downed state before auto-revive triggers.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float DownedCountdownDuration;

	// HP fraction restored on revive (0.30 = 30%).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float ReviveHPPercent;

	// Stub for future death-without-revive logic (bloodworms exhausted).
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool bOutOfBloodworms;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool bIsAlive;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool bIsDowned;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDowned OnDowned;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnRevived OnRevived;

	// Broadcast only when bOutOfBloodworms is true — not used in MVP.
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeath OnDeath;

	// Apply damage. Returns true if the actor entered the downed state.
	UFUNCTION(BlueprintCallable, Category = "Health")
	bool ApplyDamage(float Damage);

	// Remaining seconds in the downed countdown; 0 if not downed.
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetDownedTimeRemaining() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHPPercent() const { return HPMax > 0.f ? HPCurrent / HPMax : 0.f; }

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle DownedTimer;

	void EnterDowned();
	void Revive();
};
