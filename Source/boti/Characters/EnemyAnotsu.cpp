#include "Characters/EnemyAnotsu.h"
#include "Components/AC_Health.h"
#include "Components/AC_Posture.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

AEnemyAnotsu::AEnemyAnotsu()
{
	CurrentPhase               = 1;
	PhaseThreshold             = 0.40f;
	Phase2AttackSpeedMultiplier = 1.30f;
	bEscapeTriggered           = false;
	bPhaseTwoTriggered         = false;

	// Long Ko-nigiritō halberd reach — punishes careless aggression
	AttackRange         = 350.f;
	AttackDamage        = 30.f;
	AttackPostureDamage = 30.f;

	if (HealthComponent)
	{
		HealthComponent->HPMax     = 600.f;
		HealthComponent->HPCurrent = 600.f;
	}
	if (PostureComponent)
	{
		PostureComponent->PostureMax        = 100.f;
		PostureComponent->PostureCurrent    = 100.f;
		PostureComponent->PostureBreakDuration = 2.5f;
		PostureComponent->RegenRate         = 10.f;  // Slower regen rewards sustained pressure
	}
}

void AEnemyAnotsu::BeginPlay()
{
	Super::BeginPlay();
}

float AEnemyAnotsu::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
                                AController* EventInstigator, AActor* DamageCauser)
{
	if (bEscapeTriggered)
		return 0.f;

	const float Applied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!HealthComponent)
		return Applied;

	// Phase 2 trigger
	if (!bPhaseTwoTriggered && HealthComponent->GetHPPercent() < PhaseThreshold)
	{
		bPhaseTwoTriggered = true;
		SetPhase(2);
	}

	// Escape trigger — Anotsu never truly dies in act 1
	if (HealthComponent->HPCurrent <= 0.f && !bEscapeTriggered)
	{
		HealthComponent->HPCurrent = 1.f; // Prevent AC_Health death delegate from firing
		TriggerEscape();
	}

	return Applied;
}

void AEnemyAnotsu::SetPhase(int32 NewPhase)
{
	if (CurrentPhase == NewPhase)
		return;

	CurrentPhase = NewPhase;
	OnPhaseChanged.Broadcast(NewPhase);
	// Blueprint: update BB_Enemy "AnotsuPhase" key, play phase-transition VFX.
}

void AEnemyAnotsu::TriggerEscape()
{
	bEscapeTriggered = true;

	// Halt the behavior tree before the cutscene takes control
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		if (UBehaviorTreeComponent* BTC = AIC->FindComponentByClass<UBehaviorTreeComponent>())
			BTC->StopTree(EBTStopMode::Safe);
	}

	OnEscapeTriggered.Broadcast();
	// Blueprint / Sequencer: play escape montage → shoji smash → fade to cutscene.
}

void AEnemyAnotsu::PerformAttack_Implementation()
{
	// Blueprint selects montage based on CurrentPhase:
	// Phase 1 → methodical halberd sweep, 0.3s red-flash telegraph, slow recovery
	// Phase 2 → rapid flurry montage at Phase2AttackSpeedMultiplier playback rate
}
