#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FlurryStrike.generated.h"

// Anotsu Phase 2: rapid flurry combo.
// Closes distance aggressively, fires multiple fast hits.
// AEnemyAnotsu::Phase2AttackSpeedMultiplier is applied to the montage playback rate in Blueprint.
UCLASS()
class BOTI_API UBTTask_FlurryStrike : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FlurryStrike();

	// Number of rapid hits in the flurry
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 FlurryHitCount;

	// Delay between each hit in the flurry
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float FlurryHitInterval;

	// Short recovery before the task completes and the BT re-evaluates
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float RecoveryDelay;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY()
	UBehaviorTreeComponent* ActiveOwner;

	int32 HitsRemaining;
	FTimerHandle HitTimer;
	FTimerHandle RecoveryTimer;

	void FireNextHit();
	void OnRecoveryComplete();
};
