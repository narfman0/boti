#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_EnemyPerception.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDetected, AActor*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerLost);

// Perception component shared by all enemy characters.
// Polls at 10 Hz — configurable via SightRadius / HearingRadius / PeripheralVisionHalfAngleDeg.
// Dispatches OnPlayerDetected / OnPlayerLost so the AI controller can update the blackboard.
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOTI_API UAC_EnemyPerception : public UActorComponent
{
	GENERATED_BODY()

public:
	UAC_EnemyPerception();

	// Max range for line-of-sight detection
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	float SightRadius;

	// Player stays "detected" until this distance is exceeded
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	float LoseSightRadius;

	// No LoS check — sound reaches around corners
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	float HearingRadius;

	// Half-angle of the forward vision cone in degrees (90 = 180° total FOV)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	float PeripheralVisionHalfAngleDeg;

	UPROPERTY(BlueprintReadOnly, Category = "Perception")
	bool bPlayerDetected;

	// Last confirmed player location — used by the BT when sight is lost
	UPROPERTY(BlueprintReadOnly, Category = "Perception")
	FVector LastKnownPlayerLocation;

	UPROPERTY(BlueprintAssignable, Category = "Perception")
	FOnPlayerDetected OnPlayerDetected;

	UPROPERTY(BlueprintAssignable, Category = "Perception")
	FOnPlayerLost OnPlayerLost;

	UFUNCTION(BlueprintPure, Category = "Perception")
	AActor* GetDetectedPlayer() const { return DetectedPlayer; }

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	AActor* DetectedPlayer;

	bool CheckSight(AActor* Candidate) const;
	bool CheckHearing(AActor* Candidate) const;
};
