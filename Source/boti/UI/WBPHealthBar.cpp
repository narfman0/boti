#include "UI/WBPHealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/Overlay.h"
#include "GameFramework/Character.h"

void UWBPHealthBar::NativeConstruct()
{
	Super::NativeConstruct();

	if (OwningCharacter)
	{
		UAC_Health* Health = OwningCharacter->FindComponentByClass<UAC_Health>();
		if (Health)
			BindToHealthComponent(Health);
	}

	if (DownedVignetteOverlay)
		DownedVignetteOverlay->SetVisibility(ESlateVisibility::Hidden);
}

void UWBPHealthBar::BindToHealthComponent(UAC_Health* Health)
{
	if (!Health)
		return;

	HealthComp = Health;
	HealthComp->OnDowned.AddDynamic(this, &UWBPHealthBar::HandleDowned);
	HealthComp->OnRevived.AddDynamic(this, &UWBPHealthBar::HandleRevived);
}

float UWBPHealthBar::GetHealthPercent() const
{
	return HealthComp ? HealthComp->GetHPPercent() : 1.f;
}

void UWBPHealthBar::HandleDowned()
{
	if (DownedVignetteOverlay)
		DownedVignetteOverlay->SetVisibility(ESlateVisibility::Visible);
}

void UWBPHealthBar::HandleRevived()
{
	if (DownedVignetteOverlay)
		DownedVignetteOverlay->SetVisibility(ESlateVisibility::Hidden);
}
