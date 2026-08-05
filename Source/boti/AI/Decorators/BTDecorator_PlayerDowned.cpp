#include "AI/Decorators/BTDecorator_PlayerDowned.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/BotiBlackboardKeys.h"

UBTDecorator_PlayerDowned::UBTDecorator_PlayerDowned()
{
	NodeName = TEXT("Player Is Downed");
}

bool UBTDecorator_PlayerDowned::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	return BB ? BB->GetValueAsBool(BBKeys::bTargetDowned) : false;
}
