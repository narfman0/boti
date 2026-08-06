#include "UI/WBPPostureBar.h"
#include "Components/ProgressBar.h"
#include "GameFramework/Character.h"

void UWBPPostureBar::NativeConstruct()
{
	Super::NativeConstruct();

	if (OwningCharacter)
	{
		UAC_Posture* Posture = OwningCharacter->FindComponentByClass<UAC_Posture>();
		if (Posture)
			BindToPostureComponent(Posture);
	}
}

void UWBPPostureBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (PostureProgressBar && PostureComp)
	{
		PostureProgressBar->SetPercent(PostureComp->GetPosturePercent());

		// Drive red tint when critical — Blueprint can also bind to IsPostureCritical().
		FLinearColor BarColor = IsPostureCritical()
			? FLinearColor(1.f, 0.1f, 0.1f, 1.f)
			: FLinearColor(0.8f, 0.8f, 0.2f, 1.f);
		PostureProgressBar->SetFillColorAndOpacity(BarColor);
	}
}

void UWBPPostureBar::BindToPostureComponent(UAC_Posture* Posture)
{
	PostureComp = Posture;
}

float UWBPPostureBar::GetPosturePercent() const
{
	return PostureComp ? PostureComp->GetPosturePercent() : 1.f;
}

bool UWBPPostureBar::IsPostureCritical() const
{
	return PostureComp ? PostureComp->GetPosturePercent() < 0.25f : false;
}
