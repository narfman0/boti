#include "BotiCharacter.h"
#include "Components/AC_Health.h"
#include "Components/AC_LockOn.h"
#include "Components/AC_Posture.h"
#include "Components/AC_PostProcess.h"
#include "Combat/AC_HitStop.h"
#include "VFX/BotiVFXLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"

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

	HealthComponent       = CreateDefaultSubobject<UAC_Health>(TEXT("HealthComponent"));
	PostureComponent      = CreateDefaultSubobject<UAC_Posture>(TEXT("PostureComponent"));
	LockOnComponent       = CreateDefaultSubobject<UAC_LockOn>(TEXT("LockOnComponent"));
	HitStopComponent      = CreateDefaultSubobject<UAC_HitStop>(TEXT("HitStopComponent"));
	PostProcessComponent  = CreateDefaultSubobject<UAC_PostProcess>(TEXT("PostProcessComponent"));

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
	BloodwormVFX          = nullptr;
}

void ABotiCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnDowned.AddDynamic(this, &ABotiCharacter::OnDownedCallback);
		HealthComponent->OnRevived.AddDynamic(this, &ABotiCharacter::OnRevivedCallback);
	}
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

		if (HitStopComponent)
			HitStopComponent->ApplyParryHitStop();

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

	if (HealthComponent && (!HealthComponent->bIsAlive || HealthComponent->bIsDowned))
		return 0.f;

	FVector HitLocation = GetActorLocation();
	float FinalDamage = HandleIncomingHit(DamageCauser, DamageAmount, HitLocation);

	if (HealthComponent)
		HealthComponent->ApplyDamage(FinalDamage);

	// Normal (unguarded) hit — apply short hit-stop.
	if (FinalDamage > 0.f && HitStopComponent)
		HitStopComponent->ApplyNormalHitStop();

	return FinalDamage;
}

// ---------------------------------------------------------------------------
// Downed / revive callbacks
// ---------------------------------------------------------------------------

void ABotiCharacter::OnDownedCallback()
{
	bParryActive      = false;
	bIsBlocking       = false;
	bRiposteAvailable = false;

	GetWorldTimerManager().ClearTimer(ParryWindowTimer);
	GetWorldTimerManager().ClearTimer(RiposteWindowTimer);

	if (BloodwormVFX && GetWorld())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), BloodwormVFX, GetActorLocation());
	}

	if (PostProcessComponent)
		PostProcessComponent->SetDownedEffect(true);
}

void ABotiCharacter::OnRevivedCallback()
{
	if (PostProcessComponent)
		PostProcessComponent->SetDownedEffect(false);
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
	// Prefer the VFX library soft-ref (set via DefaultGame.ini or Blueprint CDO).
	UBotiVFXLibrary::SpawnParrySpark(this, Location);

	// Fallback: direct Niagara system assigned in the character Blueprint.
	if (ParrySparkVFX && GetWorld())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), ParrySparkVFX, Location);
	}
}

// ---------------------------------------------------------------------------
// Enhanced Input
// ---------------------------------------------------------------------------

void ABotiCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Sub =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
				Sub->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (IA_Move)         EIC->BindAction(IA_Move,        ETriggerEvent::Triggered, this, &ABotiCharacter::Move);
		if (IA_Look)         EIC->BindAction(IA_Look,        ETriggerEvent::Triggered, this, &ABotiCharacter::Look);
		if (IA_LightAttack)  EIC->BindAction(IA_LightAttack, ETriggerEvent::Started,   this, &ABotiCharacter::LightAttack);
		if (IA_HeavyAttack)  EIC->BindAction(IA_HeavyAttack, ETriggerEvent::Started,   this, &ABotiCharacter::HeavyAttack);
		if (IA_Dodge)        EIC->BindAction(IA_Dodge,       ETriggerEvent::Started,   this, &ABotiCharacter::DodgeRoll);
		if (IA_Parry)        EIC->BindAction(IA_Parry,       ETriggerEvent::Started,   this, &ABotiCharacter::Parry);
		if (IA_Block)        EIC->BindAction(IA_Block,       ETriggerEvent::Started,   this, &ABotiCharacter::StartBlock);
		if (IA_Block)        EIC->BindAction(IA_Block,       ETriggerEvent::Completed,  this, &ABotiCharacter::EndBlock);
		if (IA_LockOn)       EIC->BindAction(IA_LockOn,      ETriggerEvent::Started,   this, &ABotiCharacter::ToggleLockOn);
		if (IA_Riposte)      EIC->BindAction(IA_Riposte,     ETriggerEvent::Started,   this, &ABotiCharacter::Riposte);
	}
}

void ABotiCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MoveVec = Value.Get<FVector2D>();
	if (!Controller) return;
	const FRotator Yaw(0, Controller->GetControlRotation().Yaw, 0);
	AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::X), MoveVec.Y);
	AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y), MoveVec.X);
}

void ABotiCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVec = Value.Get<FVector2D>();
	AddControllerYawInput(LookVec.X);
	AddControllerPitchInput(LookVec.Y);
}
