#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FindPlayer.generated.h"

// BT service that runs at the root of all enemy trees.
// Ticks every 0.2s to keep TargetActor and bTargetDowned in sync.
// Interval can be loosened once perception is trusted; keeping it tight
// ensures downed-state reactions fire within one tick of the player going down.
UCLASS()
class BOTI_API UBTService_FindPlayer : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_FindPlayer();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
