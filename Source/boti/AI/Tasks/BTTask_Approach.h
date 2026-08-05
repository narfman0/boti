#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Approach.generated.h"

// Moves the enemy toward TargetActor until within the enemy's AttackRange.
// Succeeds once in range; fails if the target is lost.
UCLASS()
class BOTI_API UBTTask_Approach : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Approach();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
