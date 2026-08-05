#include "AI/Tasks/BTTask_BackOff.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Characters/EnemyGrunt.h"
#include "AI/BotiBlackboardKeys.h"

UBTTask_BackOff::UBTTask_BackOff()
{
	NodeName            = TEXT("Back Off");
	BackOffDuration     = 1.5f;
	BackOffDistance     = 250.f;
	ActiveOwner         = nullptr;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_BackOff::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	APawn* Pawn        = AIC ? AIC->GetPawn() : nullptr;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	if (!AIC || !Pawn || !BB)
		return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(BBKeys::TargetActor));

	// Move to a point behind the enemy (away from the player)
	FVector BackDir   = Target
		? (Pawn->GetActorLocation() - Target->GetActorLocation()).GetSafeNormal()
		: -Pawn->GetActorForwardVector();
	FVector BackPoint = Pawn->GetActorLocation() + BackDir * BackOffDistance;

	AIC->MoveToLocation(BackPoint, 50.f);

	// Notify the grunt so Blueprint can play the retreat animation
	if (AEnemyGrunt* Grunt = Cast<AEnemyGrunt>(Pawn))
		Grunt->BeginBackOff();

	ActiveOwner = &OwnerComp;
	Pawn->GetWorldTimerManager().SetTimer(
		BackOffTimer, this, &UBTTask_BackOff::OnBackOffComplete, BackOffDuration, false);

	return EBTNodeResult::InProgress;
}

void UBTTask_BackOff::OnBackOffComplete()
{
	if (!ActiveOwner)
		return;

	if (AAIController* AIC = ActiveOwner->GetAIOwner())
		AIC->StopMovement();

	FinishLatentTask(*ActiveOwner, EBTNodeResult::Succeeded);
	ActiveOwner = nullptr;
}

EBTNodeResult::Type UBTTask_BackOff::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIC = OwnerComp.GetAIOwner())
	{
		if (APawn* Pawn = AIC->GetPawn())
			Pawn->GetWorldTimerManager().ClearTimer(BackOffTimer);
		AIC->StopMovement();
	}
	ActiveOwner = nullptr;
	return EBTNodeResult::Aborted;
}
