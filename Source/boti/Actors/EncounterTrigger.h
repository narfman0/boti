#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EncounterTrigger.generated.h"

class UBoxComponent;
class AArenaGate;

UCLASS()
class BOTI_API AEncounterTrigger : public AActor
{
	GENERATED_BODY()

public:
	AEncounterTrigger();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* TriggerVolume;

	UPROPERTY(BlueprintReadOnly)
	bool bTriggered = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<ACharacter>> EnemyClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> SpawnOffsets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AArenaGate* LinkedGate;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
