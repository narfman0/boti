#include "Components/AC_WinSequence.h"
#include "Camera/CameraActor.h"
#include "Engine/PostProcessVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShakeBase.h"
#include "Components/MeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

UAC_WinSequence::UAC_WinSequence()
{
	PrimaryComponentTick.bCanEverTick = true;

	bFadingAnotsu    = false;
	AnotsuFadeElapsed = 0.f;
	AnotsuFadeDuration = 2.f;

	bFadingToBlack   = false;
	BlackFadeElapsed  = 0.f;
	BlackFadeDuration = 1.f;
}

void UAC_WinSequence::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}

void UAC_WinSequence::PlayEscapeSequence(AActor* Anotsu)
{
	if (!Anotsu)
		return;

	FadingAnotsu      = Anotsu;
	bFadingAnotsu     = true;
	AnotsuFadeElapsed = 0.f;
	AnotsuFadeDuration = 2.f;

	// Switch to escape camera if assigned
	if (EscapeCamera)
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
		{
			PC->SetViewTargetWithBlend(EscapeCamera, 0.5f);
		}
	}

	// Cinematic camera shake
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->ClientStartCameraShake(UCameraShakeBase::StaticClass(), 1.0f);
	}

	SetComponentTickEnabled(true);
}

void UAC_WinSequence::FadeToBlack(float Duration)
{
	bFadingToBlack   = true;
	BlackFadeElapsed  = 0.f;
	BlackFadeDuration = FMath::Max(Duration, 0.01f);
	SetComponentTickEnabled(true);
}

void UAC_WinSequence::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Fade Anotsu out over AnotsuFadeDuration — drives per-mesh material opacity if
	// dynamic material instances expose an "Opacity" scalar; hides the actor when done.
	if (bFadingAnotsu && FadingAnotsu.IsValid())
	{
		AnotsuFadeElapsed += DeltaTime;
		const float Alpha = FMath::Clamp(AnotsuFadeElapsed / AnotsuFadeDuration, 0.f, 1.f);

		// Try to drive material "Opacity" scalar on any MIDs already created
		TArray<UMeshComponent*> Meshes;
		FadingAnotsu->GetComponents<UMeshComponent>(Meshes);
		for (UMeshComponent* Mesh : Meshes)
		{
			for (int32 i = 0; i < Mesh->GetNumMaterials(); ++i)
			{
				if (UMaterialInstanceDynamic* MID = Cast<UMaterialInstanceDynamic>(Mesh->GetMaterial(i)))
				{
					MID->SetScalarParameterValue(TEXT("Opacity"), 1.f - Alpha);
				}
			}
		}

		if (Alpha >= 1.f)
		{
			FadingAnotsu->SetActorHiddenInGame(true);
			bFadingAnotsu = false;
			FinishEscapeSequence();
		}
	}

	// Lerp post-process volume weight to 1 (pitch black)
	if (bFadingToBlack && FadePostProcess)
	{
		BlackFadeElapsed += DeltaTime;
		const float Alpha = FMath::Clamp(BlackFadeElapsed / BlackFadeDuration, 0.f, 1.f);
		FadePostProcess->BlendWeight = Alpha;

		if (Alpha >= 1.f)
			bFadingToBlack = false;
	}

	if (!bFadingAnotsu && !bFadingToBlack)
		SetComponentTickEnabled(false);
}

void UAC_WinSequence::FinishEscapeSequence()
{
	FadeToBlack(1.5f);
	OnEscapeComplete.Broadcast();
}
