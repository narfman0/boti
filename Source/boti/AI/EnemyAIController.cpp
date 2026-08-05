#include "AI/EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EnemyCharacter.h"
#include "Components/AC_EnemyPerception.h"
#include "AI/BotiBlackboardKeys.h"

AEnemyAIController::AEnemyAIController()
{
	BehaviorTreeAsset = nullptr;
	BlackboardAsset   = nullptr;
	CachedPerception  = nullptr;
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset && BlackboardAsset)
	{
		UBlackboardComponent* BB = nullptr;
		UseBlackboard(BlackboardAsset, BB);
		RunBehaviorTree(BehaviorTreeAsset);
	}

	// Wire perception delegates so detection events flow into the blackboard
	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(InPawn))
	{
		CachedPerception = Enemy->PerceptionComponent;
		if (CachedPerception)
		{
			CachedPerception->OnPlayerDetected.AddDynamic(
				this, &AEnemyAIController::OnPlayerDetectedCallback);
			CachedPerception->OnPlayerLost.AddDynamic(
				this, &AEnemyAIController::OnPlayerLostCallback);
		}
	}
}

void AEnemyAIController::OnUnPossess()
{
	if (CachedPerception)
	{
		CachedPerception->OnPlayerDetected.RemoveDynamic(
			this, &AEnemyAIController::OnPlayerDetectedCallback);
		CachedPerception->OnPlayerLost.RemoveDynamic(
			this, &AEnemyAIController::OnPlayerLostCallback);
		CachedPerception = nullptr;
	}

	Super::OnUnPossess();
}

void AEnemyAIController::NotifyPlayerDetected(AActor* Player)
{
	if (Blackboard)
		Blackboard->SetValueAsObject(BBKeys::TargetActor, Player);
}

void AEnemyAIController::NotifyPlayerLost()
{
	if (Blackboard)
		Blackboard->SetValueAsObject(BBKeys::TargetActor, nullptr);
}

void AEnemyAIController::OnPlayerDetectedCallback(AActor* Player)
{
	NotifyPlayerDetected(Player);
}

void AEnemyAIController::OnPlayerLostCallback()
{
	NotifyPlayerLost();
}
