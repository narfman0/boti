#include "BotiCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABotiCharacter::ABotiCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.f, 80.f, 30.f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->FieldOfView = 75.f;

	MoveSpeed = 600.f;
	bIsLockedOn = false;
	bIsInvincible = false;
}

void ABotiCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABotiCharacter::LightAttack()
{
	// Blueprint implements animation montage
}

void ABotiCharacter::HeavyAttack()
{
	// Blueprint implements animation montage
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
}

void ABotiCharacter::EndInvincibility()
{
	bIsInvincible = false;
}
