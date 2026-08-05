#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class UAC_Health;
class UAC_Posture;
class UAC_EnemyPerception;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDied);

// Base class for all boti enemy characters.
// Carries Health, Posture, and Perception components.
// Blueprint subclasses (BP_Enemy_Grunt, BP_Enemy_Shira, BP_Enemy_Anotsu) assign
// meshes, montages, and the AI controller class.
UCLASS()
class BOTI_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	// --- Components ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UAC_Health* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UAC_Posture* PostureComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UAC_EnemyPerception* PerceptionComponent;

	// --- Patrol ---

	// Waypoint actors assigned in the level editor (e.g., via BP_PatrolPath spline targets)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol")
	TArray<AActor*> PatrolPoints;

	// Distance at which a patrol waypoint counts as reached
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol")
	float PatrolAcceptanceRadius;

	// --- Combat ---

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackDamage;

	// Posture damage dealt to the player per hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackPostureDamage;

	// --- Events ---

	UPROPERTY(BlueprintAssignable, Category = "Enemy")
	FOnEnemyDied OnEnemyDied;

	// Blueprint overrides this to select and play the correct attack montage.
	// Hit detection fires from UAnimNotify_MeleeTrace at the appropriate frame.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void PerformAttack();
	virtual void PerformAttack_Implementation();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnDiedCallback();
};
