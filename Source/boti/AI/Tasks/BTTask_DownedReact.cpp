#include "AI/Tasks/BTTask_DownedReact.h"
#include "AIController.h"
#include "Characters/EnemyGrunt.h"

UBTTask_DownedReact::UBTTask_DownedReact()
{
	NodeName            = TEXT("Downed React (Hesitate)");
	HesitationDuration  = 1.0f;
	ActiveOwner         = nullptr;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_DownedReact::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	APawn* Pawn        = AIC ? AIC->GetPawn() : nullptr;
	if (!Pawn)
		return EBTNodeResult::Failed;

	AIC->StopMovement();

	if (AEnemyGrunt* Grunt = Cast<AEnemyGrunt>(Pawn))
		Grunt->ReactToPlayerDowned();

	ActiveOwner = &OwnerComp;
	Pawn->GetWorldTimerManager().SetTimer(
		HesitationTimer, this, &UBTTask_DownedReact::OnHesitationComplete, HesitationDuration, false);

	return EBTNodeResult::InProgress;
}

void UBTTask_DownedReact::OnHesitationComplete()
{
	if (!ActiveOwner)
		return;

	FinishLatentTask(*ActiveOwner, EBTNodeResult::Succeeded);
	ActiveOwner = nullptr;
}

EBTNodeResult::Type UBTTask_DownedReact::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIC = OwnerComp.GetAIOwner())
		if (APawn* Pawn = AIC->GetPawn())
			Pawn->GetWorldTimerManager().ClearTimer(HesitationTimer);

	ActiveOwner = nullptr;
	return EBTNodeResult::Aborted;
}
