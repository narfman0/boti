#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BotiCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class BOTI_API ABotiCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABotiCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsLockedOn;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsInvincible;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void LightAttack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void HeavyAttack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void DodgeRoll();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ToggleLockOn();

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle InvincibilityTimer;
	void EndInvincibility();
};
