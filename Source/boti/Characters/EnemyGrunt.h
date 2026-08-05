#pragma once

#include "CoreMinimal.h"
#include "Characters/EnemyCharacter.h"
#include "EnemyGrunt.generated.h"

UENUM(BlueprintType)
enum class EGruntWeaponStyle : uint8
{
	Katana   UMETA(DisplayName = "Katana"),
	Naginata UMETA(DisplayName = "Naginata"),
};

// Ittō-ryū foot soldier — shared mesh, two weapon loadouts.
// BT: Patrol → Detect → Approach → AttackLoop (2-hit → BackOff 1.5s)
// On player downed: pauses 1s (fear window) before resuming attack.
UCLASS()
class BOTI_API AEnemyGrunt : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	AEnemyGrunt();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Grunt")
	EGruntWeaponStyle WeaponStyle;

	// True while stepping back after completing a 2-hit combo (BTTask_BackOff controls this)
	UPROPERTY(BlueprintReadOnly, Category = "Enemy|Grunt")
	bool bBackingOff;

	// Starts the 1.5s back-off window — called by BTTask_BackOff
	UFUNCTION(BlueprintCallable, Category = "Enemy|Grunt")
	void BeginBackOff();

	UFUNCTION(BlueprintCallable, Category = "Enemy|Grunt")
	void EndBackOff();

	// Brief 1s hesitation when Manji goes down — plays fear-flinch animation in Blueprint
	UFUNCTION(BlueprintCallable, Category = "Enemy|Grunt")
	void ReactToPlayerDowned();

	virtual void PerformAttack_Implementation() override;

private:
	FTimerHandle BackOffTimer;
};
