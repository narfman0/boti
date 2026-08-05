#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_HealthBelow.generated.h"

// Returns true when the owning enemy's HP is below Threshold (0–1 fraction).
// Used by BT_Anotsu to gate the Phase 2 selector branch at 40% HP.
UCLASS()
class BOTI_API UBTDecorator_HealthBelow : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_HealthBelow();

	// HP fraction — 0.40 = 40%
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	float Threshold;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
