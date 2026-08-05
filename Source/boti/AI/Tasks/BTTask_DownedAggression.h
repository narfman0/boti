#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DownedAggression.generated.h"

// Shira's downed-state behavior: sprint toward Manji and press the attack.
// Calls AEnemyShira::ActivateDownedAggression() to flag Blueprint for sprint animation.
// Succeeds once Shira reaches the player or the player revives (bTargetDowned becomes false).
UCLASS()
class BOTI_API UBTTask_DownedAggression : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_DownedAggression();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
