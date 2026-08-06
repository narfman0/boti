#include "UI/WBPBossBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UWBPBossBar::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);

	// Subscribe to phase changes from the game mode.
	ABotiGameMode* GM = Cast<ABotiGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM)
		GM->OnPhaseChanged.AddDynamic(this, &UWBPBossBar::HandlePhaseChanged);
}

void UWBPBossBar::BindToBoss(AActor* BossActor, const FText& BossName)
{
	if (!BossActor)
		return;

	BossPostureComp = BossActor->FindComponentByClass<UAC_Posture>();

	if (BossNameText)
		BossNameText->SetText(BossName);
}

float UWBPBossBar::GetBossPosturePercent() const
{
	return BossPostureComp ? BossPostureComp->GetPosturePercent() : 1.f;
}

void UWBPBossBar::HandlePhaseChanged(EGamePhase NewPhase)
{
	bool bBossVisible = (NewPhase == EGamePhase::InCombat ||
	                     NewPhase == EGamePhase::BossPhase2 ||
	                     NewPhase == EGamePhase::BossEscape);

	SetVisibility(bBossVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
