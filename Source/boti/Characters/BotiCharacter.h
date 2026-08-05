#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BotiCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAC_Health;
class UAC_Posture;
class UAC_LockOn;
class UNiagaraSystem;

UCLASS()
class BOTI_API ABotiCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABotiCharacter();

	// --- Camera ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	// --- Components ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UAC_Health* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UAC_Posture* PostureComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UAC_LockOn* LockOnComponent;

	// --- Movement ---

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed;

	// --- Combat state ---

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsLockedOn;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsInvincible;

	// True during the 8-frame parry window
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bParryActive;

	// True while block input is held
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsBlocking;

	// True after a successful parry, until the riposte window closes
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bRiposteAvailable;

	// The enemy staggered by the most recent parry (riposte target)
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	AActor* StaggeredTarget;

	// Damage multiplier for a successful block (default: 30% of incoming = 70% reduction)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float BlockDamageMultiplier;

	// Posture damage applied to player per blocked hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float BlockPostureCost;

	// Posture damage dealt to enemy on a successful parry
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float ParryPostureDamage;

	// Base damage for a riposte strike
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float RiposteDamage;

	// VFX spawned at parry clash point — assign in Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|VFX")
	UNiagaraSystem* ParrySparkVFX;

	// Bloodworm wound VFX played on downed — assign in Blueprint (placeholder Niagara system)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|VFX")
	UNiagaraSystem* BloodwormVFX;

	// --- Actions ---

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void LightAttack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void HeavyAttack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void DodgeRoll();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ToggleLockOn();

	// Trigger the parry window — call on parry input pressed
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Parry();

	// Hold block — call StartBlock on input pressed, EndBlock on released
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartBlock();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EndBlock();

	// Execute riposte strike against StaggeredTarget (if available)
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Riposte();

	// Execute visceral kill against an enemy whose posture is broken
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TryVisceralKill(AActor* Target);

	// Called when an incoming hit lands on this character;
	// intercepts for parry/block before applying damage.
	UFUNCTION(BlueprintCallable, Category = "Combat")
	float HandleIncomingHit(AActor* Attacker, float RawDamage, FVector HitLocation);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle InvincibilityTimer;
	FTimerHandle ParryWindowTimer;
	FTimerHandle RiposteWindowTimer;

	void EndInvincibility();
	void EndParryWindow();
	void EndRiposteWindow();

	void SpawnParrySparkVFX(FVector Location);
	void ApplyVisceralKill(AActor* Target);

	UFUNCTION()
	void OnDownedCallback();

	UFUNCTION()
	void OnRevivedCallback();
};
