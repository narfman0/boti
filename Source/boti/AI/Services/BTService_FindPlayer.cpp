#include "AI/Services/BTService_FindPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/AC_Health.h"
#include "AI/BotiBlackboardKeys.h"

UBTService_FindPlayer::UBTService_FindPlayer()
{
	NodeName     = TEXT("Find Player");
	Interval     = 0.2f;
	RandomDeviation = 0.05f;
}

void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
		return;

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!Player)
		return;

	BB->SetValueAsObject(BBKeys::TargetActor, Player);

	UAC_Health* PlayerHealth = Player->FindComponentByClass<UAC_Health>();
	const bool bDowned = PlayerHealth ? PlayerHealth->bIsDowned : false;
	BB->SetValueAsBool(BBKeys::bTargetDowned, bDowned);
}
