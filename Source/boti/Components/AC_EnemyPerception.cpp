#include "Components/AC_EnemyPerception.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

UAC_EnemyPerception::UAC_EnemyPerception()
{
	PrimaryComponentTick.bCanEverTick  = true;
	PrimaryComponentTick.TickInterval  = 0.1f; // 10 Hz — cheap polling

	SightRadius                  = 2000.f;
	LoseSightRadius              = 2400.f;
	HearingRadius                = 800.f;
	PeripheralVisionHalfAngleDeg = 90.f;
	bPlayerDetected              = false;
	LastKnownPlayerLocation      = FVector::ZeroVector;
	DetectedPlayer               = nullptr;
}

void UAC_EnemyPerception::BeginPlay()
{
	Super::BeginPlay();
}

void UAC_EnemyPerception::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!Player)
		return;

	const bool bCanSee  = CheckSight(Player);
	const bool bCanHear = CheckHearing(Player);

	if (bCanSee || bCanHear)
	{
		LastKnownPlayerLocation = Player->GetActorLocation();

		if (!bPlayerDetected)
		{
			bPlayerDetected = true;
			DetectedPlayer  = Player;
			OnPlayerDetected.Broadcast(Player);
		}
	}
	else if (bPlayerDetected)
	{
		const float DistSq = FVector::DistSquared(
			GetOwner()->GetActorLocation(), Player->GetActorLocation());

		if (DistSq > FMath::Square(LoseSightRadius))
		{
			bPlayerDetected = false;
			DetectedPlayer  = nullptr;
			OnPlayerLost.Broadcast();
		}
	}
}

bool UAC_EnemyPerception::CheckSight(AActor* Candidate) const
{
	if (!Candidate || !GetOwner() || !GetWorld())
		return false;

	const FVector OwnerLoc     = GetOwner()->GetActorLocation() + FVector(0.f, 0.f, 70.f); // eye height
	const FVector CandidateLoc = Candidate->GetActorLocation();
	const float   DistSq       = FVector::DistSquared(OwnerLoc, CandidateLoc);

	if (DistSq > FMath::Square(SightRadius))
		return false;

	// Peripheral vision cone
	const FVector ToCandidate  = (CandidateLoc - OwnerLoc).GetSafeNormal();
	const FVector OwnerForward = GetOwner()->GetActorForwardVector();
	const float   DotProduct   = FVector::DotProduct(OwnerForward, ToCandidate);
	const float   HalfAngleRad = FMath::DegreesToRadians(PeripheralVisionHalfAngleDeg);
	if (DotProduct < FMath::Cos(HalfAngleRad))
		return false;

	// Line of sight
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	Params.AddIgnoredActor(Candidate);
	return !GetWorld()->LineTraceSingleByChannel(Hit, OwnerLoc, CandidateLoc, ECC_Visibility, Params);
}

bool UAC_EnemyPerception::CheckHearing(AActor* Candidate) const
{
	if (!Candidate || !GetOwner())
		return false;

	const float DistSq = FVector::DistSquared(
		GetOwner()->GetActorLocation(), Candidate->GetActorLocation());

	return DistSq <= FMath::Square(HearingRadius);
}
