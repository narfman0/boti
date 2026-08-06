#include "Actors/EncounterTrigger.h"
#include "Actors/ArenaGate.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

AEncounterTrigger::AEncounterTrigger()
{
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetBoxExtent(FVector(200.f, 200.f, 100.f));
	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = TriggerVolume;
	LinkedGate = nullptr;
}

void AEncounterTrigger::BeginPlay()
{
	Super::BeginPlay();
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AEncounterTrigger::OnTriggerOverlap);
}

void AEncounterTrigger::OnTriggerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (bTriggered || !OtherActor) return;

	ACharacter* AsChar = Cast<ACharacter>(OtherActor);
	if (!AsChar || !AsChar->IsPlayerControlled()) return;

	bTriggered = true;

	TArray<AActor*> Spawned;
	UWorld* World = GetWorld();
	if (!World) return;

	for (int32 i = 0; i < EnemyClasses.Num(); ++i)
	{
		if (!EnemyClasses[i]) continue;
		FVector SpawnLoc = GetActorLocation() + (SpawnOffsets.IsValidIndex(i) ? SpawnOffsets[i] : FVector::ZeroVector);
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* Enemy = World->SpawnActor<AActor>(EnemyClasses[i], SpawnLoc, FRotator::ZeroRotator, Params);
		if (Enemy) Spawned.Add(Enemy);
	}

	if (LinkedGate && Spawned.Num() > 0)
	{
		LinkedGate->LockGate(Spawned.Num());
	}
}
