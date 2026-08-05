#include "AI/Tasks/BTTask_Approach.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Characters/EnemyCharacter.h"
#include "AI/BotiBlackboardKeys.h"

UBTTask_Approach::UBTTask_Approach()
{
	NodeName    = TEXT("Approach Player");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Approach::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC     = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	if (!AIC || !BB)
		return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(BBKeys::TargetActor));
	if (!Target)
		return EBTNodeResult::Failed;

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AIC->GetPawn());
	const float Acceptance = Enemy ? Enemy->AttackRange * 0.9f : 150.f;

	AIC->MoveToActor(Target, Acceptance);
	return EBTNodeResult::InProgress;
}

void UBTTask_Approach::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIC     = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	if (!AIC || !BB)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(BBKeys::TargetActor));
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AIC->GetPawn());

	if (!Target || !Enemy)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const float Dist = FVector::Dist(Enemy->GetActorLocation(), Target->GetActorLocation());
	if (Dist <= Enemy->AttackRange)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

EBTNodeResult::Type UBTTask_Approach::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIC = OwnerComp.GetAIOwner())
		AIC->StopMovement();

	return EBTNodeResult::Aborted;
}
