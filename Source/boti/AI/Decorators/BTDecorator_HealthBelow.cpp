#include "AI/Decorators/BTDecorator_HealthBelow.h"
#include "Components/AC_Health.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"

UBTDecorator_HealthBelow::UBTDecorator_HealthBelow()
{
	NodeName  = TEXT("Health Below Threshold");
	Threshold = 0.40f;
}

bool UBTDecorator_HealthBelow::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const APawn* Pawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (!Pawn)
		return false;

	const UAC_Health* Health = Pawn->FindComponentByClass<UAC_Health>();
	return Health ? Health->GetHPPercent() < Threshold : false;
}
