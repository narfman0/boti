#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

class USplineComponent;

UCLASS()
class BOTI_API APatrolPath : public AActor
{
	GENERATED_BODY()

public:
	APatrolPath();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USplineComponent* Spline;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetPatrolPoint(int32 Index) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetNumPoints() const;
};
