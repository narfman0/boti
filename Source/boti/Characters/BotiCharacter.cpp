#include "BotiCharacter.h"
#include "Components/AC_LockOn.h"
#include "Components/AC_Posture.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

ABotiCharacter::ABotiCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw   = false;
	bUseControllerRotationRoll  = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate              = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed              = 600.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength         = 300.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset            = FVector(0.f, 80.f, 30.f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->FieldOfView             = 75.f;

	PostureComponent = CreateDefaultSubobject<UAC_Posture>(TEXT("PostureComponent"));
	LockOnComponent  = CreateDefaultSubobject<UAC_LockOn>(TEXT("LockOnComponent"));

	MoveSpeed           = 600.f;
	bIsLockedOn         = false;
	bIsInvincible       = false;
	bParryActive        = false;
	bIsBlocking         = false;
	bRiposteAvailable   = false;
	StaggeredTarget     = nullptr;
	BlockDamageMultiplier = 0.3f;
	BlockPostureCost      = 20.f;
	ParryPostureDamage    = 50.f;
	RiposteDamage         = 80.f;
	ParrySparkVFX         = nullptr;
}

void ABotiCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// ---------------------------------------------------------------------------
// Basic attacks / movement
// ---------------------------------------------------------------------------

void ABotiCharacter::LightAttack()
{
	// Blueprint drives the animation montage.
}

void ABotiCharacter::HeavyAttack()
{
	// Blueprint drives the animation montage.
}

void ABotiCharacter::DodgeRoll()
{
	FVector DodgeDir = GetActorForwardVector();
	LaunchCharacter(DodgeDir * 800.f, true, false);

	bIsInvincible = true;
	GetWorldTimerManager().SetTimer(InvincibilityTimer, this, &ABotiCharacter::EndInvincibility, 0.25f, false);
}

void ABotiCharacter::ToggleLockOn()
{
	bIsLockedOn = !bIsLockedOn;
	if (LockOnComponent)
		LockOnComponent->ToggleLock();
}

// ---------------------------------------------------------------------------
// Parry
// ---------------------------------------------------------------------------

void ABotiCharacter::Parry()
{
	if (bParryActive || bIsBlocking)
		return;

	bParryActive = true;
	// 8 frames at 60 fps ≈ 0.133 s
	GetWorldTimerManager().SetTimer(
		ParryWindowTimer, this, &ABotiCharacter::EndParryWindow, 0.133f, false);
}

void ABotiCharacter::EndParryWindow()
{
	bParryActive = false;
}

// ---------------------------------------------------------------------------
// Block
// ---------------------------------------------------------------------------

void ABotiCharacter::StartBlock()
{
	if (bParryActive)
		return;
	bIsBlocking = true;
}

void ABotiCharacter::EndBlock()
{
	bIsBlocking = false;
}

// ---------------------------------------------------------------------------
// Riposte
// ---------------------------------------------------------------------------

void ABotiCharacter::Riposte()
{
	if (!bRiposteAvailable || !StaggeredTarget)
		return;

	bRiposteAvailable = false;
	GetWorldTimerManager().ClearTimer(RiposteWindowTimer);

	// Deal riposte damage to the staggered target.
	UGameplayStatics::ApplyDamage(
		StaggeredTarget, RiposteDamage, GetController(), this, nullptr);

	StaggeredTarget = nullptr;
}

void ABotiCharacter::EndRiposteWindow()
{
	bRiposteAvailable = false;
	StaggeredTarget   = nullptr;
}

// ---------------------------------------------------------------------------
// Visceral kill
// ---------------------------------------------------------------------------

void ABotiCharacter::TryVisceralKill(AActor* Target)
{
	if (!Target)
		return;

	UAC_Posture* TargetPosture = Target->FindComponentByClass<UAC_Posture>();
	if (!TargetPosture || !TargetPosture->IsPostureBroken())
		return;

	// Mark the target as being executed so AC_Posture ignores further input.
	TargetPosture->CombatState = ECombatState::Executing;

	ApplyVisceralKill(Target);
}

void ABotiCharacter::ApplyVisceralKill(AActor* Target)
{
	// Blueprint should override or listen to OnPostureBreak to play the execution montage.
	// Here we apply lethal damage directly; a real montage callback would do this at the hit frame.
	UGameplayStatics::ApplyDamage(Target, 9999.f, GetController(), this, nullptr);
}

// ---------------------------------------------------------------------------
// Hit interception — parry / block
// ---------------------------------------------------------------------------

float ABotiCharacter::HandleIncomingHit(AActor* Attacker, float RawDamage, FVector HitLocation)
{
	// Parry window: absorb the hit, stagger the attacker, open riposte window.
	if (bParryActive)
	{
		GetWorldTimerManager().ClearTimer(ParryWindowTimer);
		bParryActive = false;

		SpawnParrySparkVFX(HitLocation);

		if (Attacker)
		{
			UAC_Posture* AttackerPosture = Attacker->FindComponentByClass<UAC_Posture>();
			if (AttackerPosture)
			{
				AttackerPosture->EnterStagger();

				if (AttackerPosture->IsPostureBroken())
				{
					// Let the player know a visceral is available via OnPostureBreak delegate.
				}
			}

			// Open riposte window for the stagger duration.
			bRiposteAvailable = true;
			StaggeredTarget   = Attacker;
			GetWorldTimerManager().SetTimer(
				RiposteWindowTimer, this, &ABotiCharacter::EndRiposteWindow,
				PostureComponent ? PostureComponent->StaggerDuration : 1.5f, false);
		}

		// Parry absorbs all damage.
		return 0.f;
	}

	// Block: reduce damage by BlockDamageMultiplier, cost player posture.
	if (bIsBlocking)
	{
		if (PostureComponent)
		{
			bool bBroke = PostureComponent->ApplyPostureDamage(BlockPostureCost);
			if (bBroke)
			{
				// Guard broken — player enters PostureBreak; Blueprint handles animation.
				bIsBlocking = false;
			}
		}
		return RawDamage * BlockDamageMultiplier;
	}

	// Unguarded hit.
	return RawDamage;
}

float ABotiCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
                                 AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsInvincible)
		return 0.f;

	FVector HitLocation = GetActorLocation();
	float FinalDamage = HandleIncomingHit(DamageCauser, DamageAmount, HitLocation);

	return Super::TakeDamage(FinalDamage, DamageEvent, EventInstigator, DamageCauser);
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

void ABotiCharacter::EndInvincibility()
{
	bIsInvincible = false;
}

void ABotiCharacter::SpawnParrySparkVFX(FVector Location)
{
	if (ParrySparkVFX && GetWorld())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), ParrySparkVFX, Location);
	}
}
