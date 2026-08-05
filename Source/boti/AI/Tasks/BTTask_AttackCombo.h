#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttackCombo.generated.h"

// Drives the grunt's 2-hit attack loop:
//   1. Call PerformAttack() on the enemy
//   2. Wait ComboHitDelay seconds
//   3. Call PerformAttack() again
//   4. Set ComboHitCount = 0, succeed — outer tree then runs BTTask_BackOff
//
// Blueprint montage notify must call PerformAttack() for hit detection;
// this task is responsible for sequencing the two attack triggers.
UCLASS()
class BOTI_API UBTTask_AttackCombo : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_AttackCombo();

	// Seconds between the first and second hit in the combo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float ComboHitDelay;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	// Stored per-node so the timer callback can finish the latent task
	UPROPERTY()
	UBehaviorTreeComponent* ActiveOwner;

	void FireSecondHit();
	void FinishCombo();

	FTimerHandle HitDelayTimer;
	FTimerHandle ComboFinishTimer;
};
