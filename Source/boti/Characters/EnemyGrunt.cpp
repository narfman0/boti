#include "Characters/EnemyGrunt.h"
#include "Components/AC_Health.h"
#include "Components/AC_Posture.h"

AEnemyGrunt::AEnemyGrunt()
{
	WeaponStyle = EGruntWeaponStyle::Katana;
	bBackingOff = false;

	AttackRange         = 180.f;
	AttackDamage        = 12.f;
	AttackPostureDamage = 18.f;

	// Naginata variant is a Blueprint child class (BP_Enemy_Grunt_Spear) that widens
	// AttackRange to 280 and slows the combo via montage playback rate.

	if (HealthComponent)
	{
		HealthComponent->HPMax     = 80.f;
		HealthComponent->HPCurrent = 80.f;
	}
	if (PostureComponent)
	{
		PostureComponent->PostureMax     = 80.f;
		PostureComponent->PostureCurrent = 80.f;
	}
}

void AEnemyGrunt::PerformAttack_Implementation()
{
	// Blueprint plays the 2-hit katana (or naginata) combo montage.
	// UAnimNotify_MeleeTrace fires hit detection at the appropriate frames.
}

void AEnemyGrunt::BeginBackOff()
{
	bBackingOff = true;
	GetWorldTimerManager().SetTimer(BackOffTimer, this, &AEnemyGrunt::EndBackOff, 1.5f, false);
}

void AEnemyGrunt::EndBackOff()
{
	bBackingOff = false;
}

void AEnemyGrunt::ReactToPlayerDowned()
{
	// Blueprint plays a brief flinch-back animation over ~1s.
	// The BT handles the timing via BTTask_DownedReact.
}
