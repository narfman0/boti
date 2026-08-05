#include "AI/Tasks/BTTask_EscapeSequence.h"
#include "AIController.h"
#include "Characters/EnemyAnotsu.h"

UBTTask_EscapeSequence::UBTTask_EscapeSequence()
{
	NodeName = TEXT("Escape Sequence (Anotsu)");
}

EBTNodeResult::Type UBTTask_EscapeSequence::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC   = OwnerComp.GetAIOwner();
	AEnemyAnotsu* Anotsu = AIC ? Cast<AEnemyAnotsu>(AIC->GetPawn()) : nullptr;
	if (!Anotsu)
		return EBTNodeResult::Failed;

	// TriggerEscape stops the BT tree internally via BehaviorTreeComponent::StopTree.
	// No need to return InProgress — the tree is halted before this return value matters.
	Anotsu->TriggerEscape();
	return EBTNodeResult::Succeeded;
}
