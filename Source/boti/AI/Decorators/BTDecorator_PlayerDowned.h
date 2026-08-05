#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_PlayerDowned.generated.h"

// Returns true when the player is currently in the downed state.
// Used to gate downed-reaction branches in BT_Grunt and BT_Shira.
// Reads bTargetDowned from the blackboard (updated by BTService_FindPlayer).
UCLASS()
class BOTI_API UBTDecorator_PlayerDowned : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_PlayerDowned();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
