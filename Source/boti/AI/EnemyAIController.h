#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UBehaviorTree;
class UBlackboardData;
class UAC_EnemyPerception;

// AI controller shared by all enemy types.
// Owns the BehaviorTreeComponent and BlackboardComponent.
// Wires AC_EnemyPerception delegates to blackboard updates on possession.
//
// Per-enemy subclasses (AShiraAIController, AAnotsuAIController) can override
// to assign different BehaviorTreeAsset / BlackboardAsset in their CDO,
// or they can be configured entirely in Blueprint child classes.
UCLASS()
class BOTI_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	// Assign matching BT and BB assets in Blueprint subclass defaults
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBlackboardData* BlackboardAsset;

	// Update TargetActor blackboard key — called by perception delegate or directly from Blueprint
	UFUNCTION(BlueprintCallable, Category = "AI")
	void NotifyPlayerDetected(AActor* Player);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void NotifyPlayerLost();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	UFUNCTION()
	void OnPlayerDetectedCallback(AActor* Player);

	UFUNCTION()
	void OnPlayerLostCallback();

	UPROPERTY()
	UAC_EnemyPerception* CachedPerception;
};
