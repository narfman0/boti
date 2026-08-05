#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_HalberdStrike.generated.h"

// Anotsu Phase 1: methodical halberd attack.
// Holds range, waits for TelegraphDelay (0.3s red-flash window), then fires the sweep.
// After the strike, backsteps to maintain optimal halberd range.
// Blueprint montage handles the actual animation and hit-detection notify.
UCLASS()
class BOTI_API UBTTask_HalberdStrike : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_HalberdStrike();

	// Seconds of telegraph before the hit lands (red flash on weapon)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float TelegraphDelay;

	// Seconds to wait after the strike before the task completes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float RecoveryDelay;

	// Distance Anotsu backsteps to after striking
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float BackstepDistance;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY()
	UBehaviorTreeComponent* ActiveOwner;

	FTimerHandle TelegraphTimer;
	FTimerHandle RecoveryTimer;

	void OnTelegraphComplete();
	void OnRecoveryComplete();
};
