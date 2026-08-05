#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DownedReact.generated.h"

// Grunt reaction when Manji goes down.
// Pauses for HesitationDuration seconds (fear window), then succeeds.
// Blueprint plays a brief flinch-back animation during this window.
// Referenced from Sprint 3 plan: "grunt backs off 1s (fear reaction)".
UCLASS()
class BOTI_API UBTTask_DownedReact : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_DownedReact();

	// Seconds to hesitate — design spec: 1s
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float HesitationDuration;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY()
	UBehaviorTreeComponent* ActiveOwner;

	FTimerHandle HesitationTimer;

	void OnHesitationComplete();
};
