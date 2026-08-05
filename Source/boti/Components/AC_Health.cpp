#include "AC_Health.h"

UAC_Health::UAC_Health()
{
	PrimaryComponentTick.bCanEverTick = false;

	HPMax                   = 100.f;
	HPCurrent               = 100.f;
	DownedCountdownDuration = 5.0f;
	ReviveHPPercent         = 0.30f;
	bOutOfBloodworms        = false;
	bIsAlive                = true;
	bIsDowned               = false;
}

void UAC_Health::BeginPlay()
{
	Super::BeginPlay();
}

bool UAC_Health::ApplyDamage(float Damage)
{
	if (!bIsAlive || bIsDowned || Damage <= 0.f)
		return false;

	HPCurrent = FMath::Max(0.f, HPCurrent - Damage);

	if (HPCurrent <= 0.f)
	{
		EnterDowned();
		return true;
	}
	return false;
}

float UAC_Health::GetDownedTimeRemaining() const
{
	if (!bIsDowned)
		return 0.f;

	return GetWorld()
		? GetWorld()->GetTimerManager().GetTimerRemaining(DownedTimer)
		: 0.f;
}

void UAC_Health::EnterDowned()
{
	bIsDowned = true;
	OnDowned.Broadcast();

	GetWorld()->GetTimerManager().SetTimer(
		DownedTimer, this, &UAC_Health::Revive, DownedCountdownDuration, false);
}

void UAC_Health::Revive()
{
	bIsDowned  = false;
	HPCurrent  = HPMax * ReviveHPPercent;
	OnRevived.Broadcast();
}
