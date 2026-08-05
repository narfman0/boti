#include "AC_LockOn.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

UAC_LockOn::UAC_LockOn()
{
	PrimaryComponentTick.bCanEverTick = true;
	LockedTarget = nullptr;
	bIsLocked = false;
}

void UAC_LockOn::BeginPlay()
{
	Super::BeginPlay();
}

void UAC_LockOn::ToggleLock()
{
	if (bIsLocked)
	{
		ReleaseLock();
		return;
	}

	AActor* Owner = GetOwner();
	if (!Owner) return;

	TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(ScanRadius);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	bool bHit = GetWorld()->OverlapMultiByChannel(
		Overlaps,
		Owner->GetActorLocation(),
		FQuat::Identity,
		ECC_Pawn,
		Sphere,
		Params
	);

	AActor* Best = nullptr;
	float BestDist = FLT_MAX;

	if (bHit)
	{
		for (const FOverlapResult& Result : Overlaps)
		{
			AActor* HitActor = Result.GetActor();
			if (!HitActor || !HitActor->ActorHasTag(FName("Enemy"))) continue;

			float Dist = FVector::DistSquared(Owner->GetActorLocation(), HitActor->GetActorLocation());
			if (Dist < BestDist)
			{
				BestDist = Dist;
				Best = HitActor;
			}
		}
	}

	if (Best)
	{
		LockedTarget = Best;
		bIsLocked = true;
	}
}

void UAC_LockOn::ReleaseLock()
{
	LockedTarget = nullptr;
	bIsLocked = false;
}

void UAC_LockOn::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsLocked || !LockedTarget) return;

	AActor* Owner = GetOwner();
	if (!Owner) return;

	// Check target still valid / alive
	if (!IsValid(LockedTarget))
	{
		ReleaseLock();
		return;
	}

	// Rotate owner toward target
	FVector ToTarget = LockedTarget->GetActorLocation() - Owner->GetActorLocation();
	FRotator TargetRot = ToTarget.Rotation();

	FRotator CurrentRot = Owner->GetActorRotation();
	FRotator NewRot = FMath::RInterpTo(CurrentRot, FRotator(CurrentRot.Pitch, TargetRot.Yaw, CurrentRot.Roll), DeltaTime, 10.f);
	Owner->SetActorRotation(NewRot);

	// Adjust controller pitch
	APawn* OwnerPawn = Cast<APawn>(Owner);
	if (OwnerPawn)
	{
		AController* Controller = OwnerPawn->GetController();
		if (Controller)
		{
			FRotator CtrlRot = Controller->GetControlRotation();
			FRotator NewCtrlRot = FMath::RInterpTo(CtrlRot, FRotator(TargetRot.Pitch * 0.5f, CtrlRot.Yaw, CtrlRot.Roll), DeltaTime, 5.f);
			Controller->SetControlRotation(NewCtrlRot);
		}
	}
}
