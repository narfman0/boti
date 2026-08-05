#include "AI/Tasks/BTTask_Patrol.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Characters/EnemyCharacter.h"
#include "AI/BotiBlackboardKeys.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_Patrol::UBTTask_Patrol()
{
	NodeName    = TEXT("Patrol");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	AEnemyCharacter* Enemy = AIC ? Cast<AEnemyCharacter>(AIC->GetPawn()) : nullptr;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	if (!AIC || !Enemy || !BB || Enemy->PatrolPoints.IsEmpty())
		return EBTNodeResult::Failed;

	const int32 Index = BB->GetValueAsInt(BBKeys::PatrolIndex);
	AActor* Waypoint = Enemy->PatrolPoints[Index % Enemy->PatrolPoints.Num()];
	if (!Waypoint)
		return EBTNodeResult::Failed;

	AIC->MoveToActor(Waypoint, Enemy->PatrolAcceptanceRadius);
	return EBTNodeResult::InProgress;
}

void UBTTask_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	AEnemyCharacter* Enemy = AIC ? Cast<AEnemyCharacter>(AIC->GetPawn()) : nullptr;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	if (!AIC || !Enemy || !BB || Enemy->PatrolPoints.IsEmpty())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const int32 Index     = BB->GetValueAsInt(BBKeys::PatrolIndex);
	const int32 SafeIndex = Index % Enemy->PatrolPoints.Num();
	AActor* Waypoint      = Enemy->PatrolPoints[SafeIndex];

	if (!Waypoint)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const float Dist = FVector::Dist(Enemy->GetActorLocation(), Waypoint->GetActorLocation());
	if (Dist <= Enemy->PatrolAcceptanceRadius)
	{
		// Advance to next waypoint, wrapping around
		BB->SetValueAsInt(BBKeys::PatrolIndex, (SafeIndex + 1) % Enemy->PatrolPoints.Num());
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UBTTask_Patrol::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIC = OwnerComp.GetAIOwner())
		AIC->StopMovement();

	return EBTNodeResult::Aborted;
}
