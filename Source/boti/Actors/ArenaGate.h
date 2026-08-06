#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArenaGate.generated.h"

class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGateOpened);

UCLASS()
class BOTI_API AArenaGate : public AActor
{
	GENERATED_BODY()

public:
	AArenaGate();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* TriggerVolume;

	UPROPERTY(BlueprintReadOnly)
	bool bGateOpen = true;

	UPROPERTY(BlueprintReadOnly)
	int32 EnemiesRemaining = 0;

	UPROPERTY(BlueprintAssignable)
	FOnGateOpened OnGateOpened;

	UFUNCTION(BlueprintCallable)
	void LockGate(int32 EnemyCount);

	UFUNCTION(BlueprintCallable)
	void OnEnemyKilled();

	UFUNCTION(BlueprintCallable)
	void OpenGate();
};
