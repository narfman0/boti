#include "Components/AC_PostProcess.h"
#include "Components/PostProcessComponent.h"
#include "GameFramework/Actor.h"

UAC_PostProcess::UAC_PostProcess()
{
	PrimaryComponentTick.bCanEverTick = true;

	VignetteLerpTime            = 0.3f;
	CombatVignetteIntensity     = 0.4f;
	DownedColorGradingIntensity = 1.f;

	CurrentVignette      = 0.f;
	TargetVignette       = 0.f;
	CurrentColorGrading  = 0.f;
	TargetColorGrading   = 0.f;
}

void UAC_PostProcess::BeginPlay()
{
	Super::BeginPlay();
	FindOrCreatePPComponent();
}

void UAC_PostProcess::FindOrCreatePPComponent()
{
	AActor* Owner = GetOwner();
	if (!Owner)
		return;

	PPComp = Owner->FindComponentByClass<UPostProcessComponent>();
	if (!PPComp)
	{
		PPComp = NewObject<UPostProcessComponent>(Owner, TEXT("BotiPostProcess"));
		PPComp->RegisterComponent();
		PPComp->bUnbound = true;
		PPComp->Priority = 1.f;
	}
}

void UAC_PostProcess::SetCombatVignette(bool bActive)
{
	TargetVignette = bActive ? CombatVignetteIntensity : 0.f;
}

void UAC_PostProcess::SetDownedEffect(bool bActive)
{
	TargetColorGrading = bActive ? DownedColorGradingIntensity : 0.f;
}

void UAC_PostProcess::TickComponent(float DeltaTime, ELevelTick TickType,
                                    FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float LerpSpeed = (VignetteLerpTime > 0.f) ? (DeltaTime / VignetteLerpTime) : 1.f;

	CurrentVignette     = FMath::FInterpTo(CurrentVignette,     TargetVignette,     DeltaTime, 1.f / FMath::Max(VignetteLerpTime, 0.001f));
	CurrentColorGrading = FMath::FInterpTo(CurrentColorGrading, TargetColorGrading, DeltaTime, 1.f / FMath::Max(VignetteLerpTime, 0.001f));

	ApplySettings();
}

void UAC_PostProcess::ApplySettings()
{
	if (!PPComp)
		return;

	FPostProcessSettings& Settings = PPComp->Settings;

	Settings.bOverride_VignetteIntensity = true;
	Settings.VignetteIntensity = CurrentVignette;

	// Downed: desaturate and apply red tint via color grading saturation + gain.
	Settings.bOverride_ColorGradingIntensity = (CurrentColorGrading > 0.f);
	Settings.ColorGradingIntensity = CurrentColorGrading;

	if (CurrentColorGrading > 0.f)
	{
		Settings.bOverride_ColorSaturation = true;
		// Lerp from neutral (1,1,1,1) to desaturated red tint
		float T = CurrentColorGrading;
		Settings.ColorSaturation = FVector4(
			FMath::Lerp(1.f, 1.2f, T),   // R slightly boosted
			FMath::Lerp(1.f, 0.5f, T),   // G reduced
			FMath::Lerp(1.f, 0.5f, T),   // B reduced
			FMath::Lerp(1.f, 0.3f, T));  // overall saturation
	}
	else
	{
		Settings.bOverride_ColorSaturation = false;
	}
}
