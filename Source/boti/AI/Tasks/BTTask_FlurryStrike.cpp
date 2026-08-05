#include "AI/Tasks/BTTask_FlurryStrike.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EnemyAnotsu.h"
#include "AI/BotiBlackboardKeys.h"

UBTTask_FlurryStrike::UBTTask_FlurryStrike()
{
	NodeName            = TEXT("Flurry Strike (Phase 2)");
	FlurryHitCount      = 4;
	FlurryHitInterval   = 0.25f; // Fast cadence — harder to read and parry
	RecoveryDelay       = 0.4f;
	ActiveOwner         = nullptr;
	HitsRemaining       = 0;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_FlurryStrike::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC   = OwnerComp.GetAIOwner();
	AEnemyAnotsu* Anotsu = AIC ? Cast<AEnemyAnotsu>(AIC->GetPawn()) : nullptr;
	if (!Anotsu)
		return EBTNodeResult::Failed;

	// Close in before the flurry
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (AActor* Target = BB ? Cast<AActor>(BB->GetValueAsObject(BBKeys::TargetActor)) : nullptr)
		AIC->MoveToActor(Target, 100.f);

	ActiveOwner  = &OwnerComp;
	HitsRemaining = FlurryHitCount;

	Anotsu->GetWorldTimerManager().SetTimer(
		HitTimer, this, &UBTTask_FlurryStrike::FireNextHit, FlurryHitInterval, false);

	return EBTNodeResult::InProgress;
}

void UBTTask_FlurryStrike::FireNextHit()
{
	if (!ActiveOwner)
		return;

	AAIController* AIC   = ActiveOwner->GetAIOwner();
	AEnemyAnotsu* Anotsu = AIC ? Cast<AEnemyAnotsu>(AIC->GetPawn()) : nullptr;
	if (!Anotsu)
	{
		FinishLatentTask(*ActiveOwner, EBTNodeResult::Failed);
		return;
	}

	AIC->StopMovement(); // Commit to the hit; Blueprint drives forward lunge in montage
	Anotsu->PerformAttack();
	--HitsRemaining;

	if (HitsRemaining > 0)
	{
		Anotsu->GetWorldTimerManager().SetTimer(
			HitTimer, this, &UBTTask_FlurryStrike::FireNextHit, FlurryHitInterval, false);
	}
	else
	{
		// All hits landed — brief recovery then succeed
		Anotsu->GetWorldTimerManager().SetTimer(
			RecoveryTimer, this, &UBTTask_FlurryStrike::OnRecoveryComplete, RecoveryDelay, false);
	}
}

void UBTTask_FlurryStrike::OnRecoveryComplete()
{
	if (!ActiveOwner)
		return;

	FinishLatentTask(*ActiveOwner, EBTNodeResult::Succeeded);
	ActiveOwner = nullptr;
}

EBTNodeResult::Type UBTTask_FlurryStrike::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (AEnemyAnotsu* Anotsu = AIC ? Cast<AEnemyAnotsu>(AIC->GetPawn()) : nullptr)
	{
		Anotsu->GetWorldTimerManager().ClearTimer(HitTimer);
		Anotsu->GetWorldTimerManager().ClearTimer(RecoveryTimer);
	}
	if (AIC)
		AIC->StopMovement();

	ActiveOwner = nullptr;
	return EBTNodeResult::Aborted;
}
