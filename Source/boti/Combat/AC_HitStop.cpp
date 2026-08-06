#include "Combat/AC_HitStop.h"
#include "Kismet/GameplayStatics.h"

UAC_HitStop::UAC_HitStop()
{
	PrimaryComponentTick.bCanEverTick = false;

	ParryHitStopDuration  = 0.05f;
	ParryTimeDilation     = 0.05f;
	NormalHitStopDuration = 0.03f;
	NormalTimeDilation    = 0.1f;
}

void UAC_HitStop::ApplyHitStop(float Duration, float TimeDilation)
{
	if (!GetWorld())
		return;

	// Cancel any in-flight hit-stop before starting a new one.
	GetWorld()->GetTimerManager().ClearTimer(HitStopTimer);

	UGameplayStatics::SetGlobalTimeDilation(this, TimeDilation);

	// Timer fires in real-world time (undilated), so scale duration accordingly.
	float RealDuration = Duration / FMath::Max(TimeDilation, SMALL_NUMBER);
	GetWorld()->GetTimerManager().SetTimer(
		HitStopTimer, this, &UAC_HitStop::RestoreTimeDilation, RealDuration, false);
}

void UAC_HitStop::ApplyParryHitStop()
{
	ApplyHitStop(ParryHitStopDuration, ParryTimeDilation);
}

void UAC_HitStop::ApplyNormalHitStop()
{
	ApplyHitStop(NormalHitStopDuration, NormalTimeDilation);
}

void UAC_HitStop::RestoreTimeDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1.f);
}
