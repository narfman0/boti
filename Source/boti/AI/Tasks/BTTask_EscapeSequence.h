#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EscapeSequence.generated.h"

// Triggers Anotsu's escape when his HP hits zero.
// Calls AEnemyAnotsu::TriggerEscape() which stops the BT and fires OnEscapeTriggered.
// This task is placed at the bottom of BT_Anotsu under a BTDecorator_HealthBelow(0.01)
// so it fires exactly once when the boss HP bottoms out.
// The task itself returns Succeeded immediately — the BT is stopped from within TriggerEscape.
UCLASS()
class BOTI_API UBTTask_EscapeSequence : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_EscapeSequence();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
