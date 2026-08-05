#include "Characters/EnemyCharacter.h"
#include "Components/AC_Health.h"
#include "Components/AC_Posture.h"
#include "Components/AC_EnemyPerception.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent     = CreateDefaultSubobject<UAC_Health>(TEXT("HealthComponent"));
	PostureComponent    = CreateDefaultSubobject<UAC_Posture>(TEXT("PostureComponent"));
	PerceptionComponent = CreateDefaultSubobject<UAC_EnemyPerception>(TEXT("PerceptionComponent"));

	PatrolAcceptanceRadius = 100.f;
	AttackRange            = 200.f;
	AttackDamage           = 15.f;
	AttackPostureDamage    = 20.f;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate              = FRotator(0.f, 480.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed              = 400.f;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
		HealthComponent->OnDeath.AddDynamic(this, &AEnemyCharacter::OnDiedCallback);
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
                                   AController* EventInstigator, AActor* DamageCauser)
{
	if (!HealthComponent || !HealthComponent->bIsAlive)
		return 0.f;

	const float Applied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (HealthComponent)
		HealthComponent->ApplyDamage(Applied);

	if (PostureComponent)
		PostureComponent->ApplyPostureDamage(AttackPostureDamage);

	return Applied;
}

void AEnemyCharacter::PerformAttack_Implementation()
{
	// Subclasses override and/or Blueprint via BlueprintNativeEvent.
}

void AEnemyCharacter::OnDiedCallback()
{
	OnEnemyDied.Broadcast();
	// Disable movement; Blueprint triggers ragdoll and blood decal stamp.
	GetCharacterMovement()->DisableMovement();
	SetActorEnableCollision(false);
}
