#include "Actors/ArenaGate.h"
#include "Components/BoxComponent.h"

AArenaGate::AArenaGate()
{
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RootComponent = TriggerVolume;
}

void AArenaGate::LockGate(int32 EnemyCount)
{
	bGateOpen = false;
	EnemiesRemaining = EnemyCount;
}

void AArenaGate::OnEnemyKilled()
{
	EnemiesRemaining = FMath::Max(0, EnemiesRemaining - 1);
	if (EnemiesRemaining == 0)
	{
		OpenGate();
	}
}

void AArenaGate::OpenGate()
{
	bGateOpen = true;
	OnGateOpened.Broadcast();
}
