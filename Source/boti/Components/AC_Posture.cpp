#include "AC_Posture.h"

UAC_Posture::UAC_Posture()
{
	PrimaryComponentTick.bCanEverTick = true;

	PostureMax           = 100.f;
	PostureCurrent       = 100.f;
	CombatState          = ECombatState::Normal;
	RegenDelay           = 2.0f;
	RegenRate            = 15.f;
	StaggerDuration      = 1.5f;
	PostureBreakDuration = 2.0f;
	HeavyHitThreshold    = 0.25f;  // >25% of PostureMax in one hit = heavy reaction
	TimeSinceLastPostureDamage = 0.f;
}

void UAC_Posture::BeginPlay()
{
	Super::BeginPlay();
}

void UAC_Posture::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CombatState == ECombatState::Normal && PostureCurrent < PostureMax)
	{
		TimeSinceLastPostureDamage += DeltaTime;
		if (TimeSinceLastPostureDamage >= RegenDelay)
		{
			PostureCurrent = FMath::Min(PostureMax, PostureCurrent + RegenRate * DeltaTime);
		}
	}
}

bool UAC_Posture::ApplyPostureDamage(float Damage)
{
	if (CombatState != ECombatState::Normal)
		return false;

	TimeSinceLastPostureDamage = 0.f;

	EHitMagnitude Magnitude = (Damage / PostureMax >= HeavyHitThreshold)
		? EHitMagnitude::Heavy
		: EHitMagnitude::Light;
	OnHitReaction.Broadcast(Magnitude);

	PostureCurrent = FMath::Max(0.f, PostureCurrent - Damage);

	if (PostureCurrent <= 0.f)
	{
		BreakPosture();
		return true;
	}
	return false;
}

void UAC_Posture::EnterStagger()
{
	// Don't interrupt a posture break or ongoing execution.
	if (CombatState == ECombatState::PostureBreak || CombatState == ECombatState::Executing)
		return;

	CombatState = ECombatState::Staggered;
	TimeSinceLastPostureDamage = 0.f;
	GetWorld()->GetTimerManager().SetTimer(
		StaggerTimer, this, &UAC_Posture::EndStagger, StaggerDuration, false);
}

void UAC_Posture::BreakPosture()
{
	CombatState    = ECombatState::PostureBreak;
	PostureCurrent = 0.f;
	OnPostureBreak.Broadcast();

	GetWorld()->GetTimerManager().SetTimer(
		PostureBreakTimer, this, &UAC_Posture::EndPostureBreak, PostureBreakDuration, false);
}

void UAC_Posture::EndStagger()
{
	if (CombatState == ECombatState::Staggered)
	{
		CombatState = ECombatState::Normal;
	}
}

void UAC_Posture::EndPostureBreak()
{
	if (CombatState == ECombatState::PostureBreak)
	{
		CombatState    = ECombatState::Normal;
		PostureCurrent = PostureMax;
		OnPostureRestored.Broadcast();
	}
}
