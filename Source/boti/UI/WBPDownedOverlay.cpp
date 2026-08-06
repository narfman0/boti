#include "UI/WBPDownedOverlay.h"
#include "Components/TextBlock.h"

void UWBPDownedOverlay::BindToHealthComponent(UAC_Health* Health)
{
	HealthComp = Health;
}

FText UWBPDownedOverlay::GetCountdownText() const
{
	if (!HealthComp || !HealthComp->bIsDowned)
		return FText::GetEmpty();

	float Remaining = HealthComp->GetDownedTimeRemaining();
	return FText::FromString(FString::Printf(TEXT("%.1f"), Remaining));
}

void UWBPDownedOverlay::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (CountdownText)
		CountdownText->SetText(GetCountdownText());
}
