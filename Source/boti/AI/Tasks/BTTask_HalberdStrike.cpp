#include "AI/Tasks/BTTask_HalberdStrike.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Characters/EnemyAnotsu.h"
#include "AI/BotiBlackboardKeys.h"

UBTTask_HalberdStrike::UBTTask_HalberdStrike()
{
	NodeName            = TEXT("Halberd Strike (Phase 1)");
	TelegraphDelay      = 0.3f;
	RecoveryDelay       = 0.8f;
	BackstepDistance    = 200.f;
	ActiveOwner         = nullptr;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_HalberdStrike::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC   = OwnerComp.GetAIOwner();
	AEnemyAnotsu* Anotsu = AIC ? Cast<AEnemyAnotsu>(AIC->GetPawn()) : nullptr;
	if (!Anotsu)
		return EBTNodeResult::Failed;

	// Stop movement — telegraph pause before the sweep
	AIC->StopMovement();
	ActiveOwner = &OwnerComp;

	// Blueprint listens to PerformAttack to start the red-flash VFX;
	// the actual hit fires after TelegraphDelay via OnTelegraphComplete.
	Anotsu->GetWorldTimerManager().SetTimer(
		TelegraphTimer, this, &UBTTask_HalberdStrike::OnTelegraphComplete, TelegraphDelay, false);

	return EBTNodeResult::InProgress;
}

void UBTTask_HalberdStrike::OnTelegraphComplete()
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

	// Fire the attack — Blueprint plays the halberd sweep montage
	Anotsu->PerformAttack();

	// Backstep after the swing
	UBlackboardComponent* BB = ActiveOwner->GetBlackboardComponent();
	AActor* Target = BB ? Cast<AActor>(BB->GetValueAsObject(BBKeys::TargetActor)) : nullptr;
	if (Target)
	{
		FVector BackDir   = (Anotsu->GetActorLocation() - Target->GetActorLocation()).GetSafeNormal();
		FVector BackPoint = Anotsu->GetActorLocation() + BackDir * BackstepDistance;
		AIC->MoveToLocation(BackPoint, 50.f);
	}

	Anotsu->GetWorldTimerManager().SetTimer(
		RecoveryTimer, this, &UBTTask_HalberdStrike::OnRecoveryComplete, RecoveryDelay, false);
}

void UBTTask_HalberdStrike::OnRecoveryComplete()
{
	if (!ActiveOwner)
		return;

	if (AAIController* AIC = ActiveOwner->GetAIOwner())
		AIC->StopMovement();

	FinishLatentTask(*ActiveOwner, EBTNodeResult::Succeeded);
	ActiveOwner = nullptr;
}

EBTNodeResult::Type UBTTask_HalberdStrike::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (AEnemyAnotsu* Anotsu = AIC ? Cast<AEnemyAnotsu>(AIC->GetPawn()) : nullptr)
	{
		Anotsu->GetWorldTimerManager().ClearTimer(TelegraphTimer);
		Anotsu->GetWorldTimerManager().ClearTimer(RecoveryTimer);
	}
	if (AIC)
		AIC->StopMovement();

	ActiveOwner = nullptr;
	return EBTNodeResult::Aborted;
}
