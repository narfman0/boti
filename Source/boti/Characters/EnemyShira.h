#pragma once

#include "CoreMinimal.h"
#include "Characters/EnemyCharacter.h"
#include "EnemyShira.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShiraArmLost);

// Shira — sadistic mid-boss on the bridge (Encounter 4).
// Single phase, aggressive close-range brawler.
// Posture breaks fast; riposing his break triggers arm-loss cosmetic.
// Unique behavior: closes distance relentlessly when Manji is downed.
UCLASS()
class BOTI_API AEnemyShira : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	AEnemyShira();

	// Set once the riposte-on-posture-break arm-loss sequence fires
	UPROPERTY(BlueprintReadOnly, Category = "Enemy|Shira")
	bool bArmLost;

	// Broadcast when the arm detaches — Blueprint swaps to one-arm attack montage set
	UPROPERTY(BlueprintAssignable, Category = "Enemy|Shira")
	FOnShiraArmLost OnArmLost;

	// True while aggressively closing on a downed Manji
	UPROPERTY(BlueprintReadOnly, Category = "Enemy|Shira")
	bool bDownedAggression;

	// Called by BTTask_DownedAggression when the player goes down
	UFUNCTION(BlueprintCallable, Category = "Enemy|Shira")
	void ActivateDownedAggression();

	UFUNCTION(BlueprintCallable, Category = "Enemy|Shira")
	void DeactivateDownedAggression();

	// Triggered by Blueprint when the player ripostes during Shira's posture break
	UFUNCTION(BlueprintCallable, Category = "Enemy|Shira")
	void TriggerArmLoss();

	virtual void PerformAttack_Implementation() override;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnPostureBreakCallback();
};
