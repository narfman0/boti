#include "Characters/EnemyShira.h"
#include "Components/AC_Health.h"
#include "Components/AC_Posture.h"

AEnemyShira::AEnemyShira()
{
	bArmLost          = false;
	bDownedAggression = false;

	// Wide, heavy hits — breaks player guard quickly
	AttackRange         = 160.f;
	AttackDamage        = 25.f;
	AttackPostureDamage = 40.f;

	if (HealthComponent)
	{
		HealthComponent->HPMax     = 350.f;
		HealthComponent->HPCurrent = 350.f;
	}
	if (PostureComponent)
	{
		// Low PostureMax — breaks fast, but that's the design (riposte spotlight fight)
		PostureComponent->PostureMax        = 60.f;
		PostureComponent->PostureCurrent    = 60.f;
		PostureComponent->PostureBreakDuration = 2.0f;
	}
}

void AEnemyShira::BeginPlay()
{
	Super::BeginPlay();

	if (PostureComponent)
		PostureComponent->OnPostureBreak.AddDynamic(this, &AEnemyShira::OnPostureBreakCallback);
}

void AEnemyShira::PerformAttack_Implementation()
{
	// Blueprint selects montage: bArmLost ? OneArmHeavy : TwoArmBrawler
}

void AEnemyShira::ActivateDownedAggression()
{
	bDownedAggression = true;
}

void AEnemyShira::DeactivateDownedAggression()
{
	bDownedAggression = false;
}

void AEnemyShira::TriggerArmLoss()
{
	if (bArmLost)
		return;

	bArmLost = true;
	OnArmLost.Broadcast();
	// Blueprint: detach RightArmMesh, spawn blood burst, switch to OneArm attack set.
}

void AEnemyShira::OnPostureBreakCallback()
{
	// Blueprint listens to enable the "riposte → arm loss" prompt for the player.
}
