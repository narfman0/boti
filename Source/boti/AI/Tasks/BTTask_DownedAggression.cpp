#include "AI/Tasks/BTTask_DownedAggression.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Characters/EnemyShira.h"
#include "Components/AC_Health.h"
#include "GameFramework/Character.h"
#include "AI/BotiBlackboardKeys.h"

UBTTask_DownedAggression::UBTTask_DownedAggression()
{
	NodeName    = TEXT("Downed Aggression (Shira)");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_DownedAggression::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC     = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	if (!AIC || !BB)
		return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(BBKeys::TargetActor));
	if (!Target)
		return EBTNodeResult::Failed;

	if (AEnemyShira* Shira = Cast<AEnemyShira>(AIC->GetPawn()))
		Shira->ActivateDownedAggression();

	AIC->MoveToActor(Target, 80.f); // Close enough to stomp/press
	return EBTNodeResult::InProgress;
}

void UBTTask_DownedAggression::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// Abort aggression immediately if the player has revived
	if (!BB->GetValueAsBool(BBKeys::bTargetDowned))
	{
		AAIController* AIC = OwnerComp.GetAIOwner();
		if (AEnemyShira* Shira = AIC ? Cast<AEnemyShira>(AIC->GetPawn()) : nullptr)
			Shira->DeactivateDownedAggression();

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UBTTask_DownedAggression::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();

	if (AEnemyShira* Shira = AIC ? Cast<AEnemyShira>(AIC->GetPawn()) : nullptr)
		Shira->DeactivateDownedAggression();

	if (AIC)
		AIC->StopMovement();

	return EBTNodeResult::Aborted;
}
