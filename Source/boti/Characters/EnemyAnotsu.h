#pragma once

#include "CoreMinimal.h"
#include "Characters/EnemyCharacter.h"
#include "EnemyAnotsu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnotsuPhaseChanged, int32, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnotsuEscapeTriggered);

// Anotsu Kagehisa — final boss, dojo encounter (Encounter 5).
// Phase 1: methodical Ko-nigiritō halberd style, punishes aggression, stays at range.
// Phase 2 (triggered at 40% HP): abandons caution, flurry combos, +30% attack speed.
// HP → 0: escape sequence — BT stops, Blueprint/Sequencer plays escape cutscene.
UCLASS()
class BOTI_API AEnemyAnotsu : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	AEnemyAnotsu();

	// 1 = Phase 1 (methodical), 2 = Phase 2 (flurry). Read by BTDecorator_HealthBelow.
	UPROPERTY(BlueprintReadOnly, Category = "Enemy|Anotsu")
	int32 CurrentPhase;

	// HP fraction that triggers the phase transition (default: 40%)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Anotsu")
	float PhaseThreshold;

	// Montage playback rate multiplier applied in Phase 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Anotsu")
	float Phase2AttackSpeedMultiplier;

	// Locked to true once the escape sequence fires — makes Anotsu immune to further damage
	UPROPERTY(BlueprintReadOnly, Category = "Enemy|Anotsu")
	bool bEscapeTriggered;

	// Broadcast on phase switch — Blueprint plays screen-shake + red vignette transition VFX
	UPROPERTY(BlueprintAssignable, Category = "Enemy|Anotsu")
	FOnAnotsuPhaseChanged OnPhaseChanged;

	// Broadcast when HP reaches 0 — Sequencer picks this up to play the escape cutscene
	UPROPERTY(BlueprintAssignable, Category = "Enemy|Anotsu")
	FOnAnotsuEscapeTriggered OnEscapeTriggered;

	// Stops the behavior tree and fires OnEscapeTriggered — called by BTTask_EscapeSequence
	UFUNCTION(BlueprintCallable, Category = "Enemy|Anotsu")
	void TriggerEscape();

	// Switches to the given phase; no-ops if already there
	UFUNCTION(BlueprintCallable, Category = "Enemy|Anotsu")
	void SetPhase(int32 NewPhase);

	UFUNCTION(BlueprintPure, Category = "Enemy|Anotsu")
	bool IsPhaseTwo() const { return CurrentPhase >= 2; }

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         AController* EventInstigator, AActor* DamageCauser) override;

	virtual void PerformAttack_Implementation() override;

protected:
	virtual void BeginPlay() override;

private:
	bool bPhaseTwoTriggered;
};
