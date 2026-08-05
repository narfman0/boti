#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BackOff.generated.h"

// Grunt post-combo back-off: step away from the player for BackOffDuration seconds.
// Calls AEnemyGrunt::BeginBackOff() so Blueprint can play the retreat animation.
UCLASS()
class BOTI_API UBTTask_BackOff : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_BackOff();

	// Seconds to maintain retreat movement (default 1.5s — matches design spec)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float BackOffDuration;

	// How far to move away from the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float BackOffDistance;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY()
	UBehaviorTreeComponent* ActiveOwner;

	FTimerHandle BackOffTimer;

	void OnBackOffComplete();
};
