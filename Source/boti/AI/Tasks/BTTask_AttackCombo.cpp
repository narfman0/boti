#include "AI/Tasks/BTTask_AttackCombo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Characters/EnemyCharacter.h"
#include "AI/BotiBlackboardKeys.h"

UBTTask_AttackCombo::UBTTask_AttackCombo()
{
	NodeName            = TEXT("Attack Combo (2-hit)");
	ComboHitDelay       = 0.6f; // Gap between hit 1 and hit 2 — matches montage timing
	ActiveOwner         = nullptr;
	bCreateNodeInstance = true;  // Each enemy owns a private copy; avoids shared timer conflicts
}

EBTNodeResult::Type UBTTask_AttackCombo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	AEnemyCharacter* Enemy = AIC ? Cast<AEnemyCharacter>(AIC->GetPawn()) : nullptr;
	if (!Enemy)
		return EBTNodeResult::Failed;

	ActiveOwner = &OwnerComp;

	// First hit
	Enemy->PerformAttack();

	// Schedule second hit and combo finish
	Enemy->GetWorldTimerManager().SetTimer(
		HitDelayTimer, this, &UBTTask_AttackCombo::FireSecondHit, ComboHitDelay, false);

	return EBTNodeResult::InProgress;
}

void UBTTask_AttackCombo::FireSecondHit()
{
	if (!ActiveOwner)
		return;

	AAIController* AIC = ActiveOwner->GetAIOwner();
	AEnemyCharacter* Enemy = AIC ? Cast<AEnemyCharacter>(AIC->GetPawn()) : nullptr;
	if (Enemy)
		Enemy->PerformAttack();

	// Short buffer after second hit before the task completes and BackOff begins
	Enemy->GetWorldTimerManager().SetTimer(
		ComboFinishTimer, this, &UBTTask_AttackCombo::FinishCombo, 0.3f, false);
}

void UBTTask_AttackCombo::FinishCombo()
{
	if (!ActiveOwner)
		return;

	if (UBlackboardComponent* BB = ActiveOwner->GetBlackboardComponent())
		BB->SetValueAsInt(BBKeys::ComboHitCount, 0);

	FinishLatentTask(*ActiveOwner, EBTNodeResult::Succeeded);
	ActiveOwner = nullptr;
}

EBTNodeResult::Type UBTTask_AttackCombo::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (AEnemyCharacter* Enemy = AIC ? Cast<AEnemyCharacter>(AIC->GetPawn()) : nullptr)
	{
		Enemy->GetWorldTimerManager().ClearTimer(HitDelayTimer);
		Enemy->GetWorldTimerManager().ClearTimer(ComboFinishTimer);
	}
	ActiveOwner = nullptr;
	return EBTNodeResult::Aborted;
}
