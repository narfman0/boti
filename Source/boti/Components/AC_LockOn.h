#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_LockOn.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOTI_API UAC_LockOn : public UActorComponent
{
	GENERATED_BODY()

public:
	UAC_LockOn();

	UPROPERTY(BlueprintReadOnly, Category = "LockOn")
	AActor* LockedTarget;

	UPROPERTY(BlueprintReadOnly, Category = "LockOn")
	bool bIsLocked;

	UFUNCTION(BlueprintCallable, Category = "LockOn")
	void ToggleLock();

	UFUNCTION(BlueprintCallable, Category = "LockOn")
	void ReleaseLock();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	static constexpr float ScanRadius = 1500.f;
};
