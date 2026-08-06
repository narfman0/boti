#include "Game/BotiGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

ABotiGameMode::ABotiGameMode()
{
	CurrentPhase = EGamePhase::Exploring;
}

void ABotiGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ABotiGameMode::SetPhase(EGamePhase NewPhase)
{
	if (CurrentPhase == NewPhase)
		return;
	CurrentPhase = NewPhase;
	OnPhaseChanged.Broadcast(NewPhase);
}

void ABotiGameMode::StartBossPhase2()
{
	SetPhase(EGamePhase::BossPhase2);
}

void ABotiGameMode::TriggerBossEscape()
{
	SetPhase(EGamePhase::BossEscape);

	// WinSequenceComponent (or Blueprint) picks up the phase change and plays the fade.
	// After the sequence completes it should call HandleVictory().
	HandleVictory();
}

void ABotiGameMode::HandleVictory()
{
	SetPhase(EGamePhase::Victory);
	OnVictory.Broadcast();

	// Disable player input
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->DisableInput(PC);
	}

	// Open end screen after 3 seconds
	GetWorldTimerManager().SetTimer(
		VictoryTimerHandle,
		this,
		&ABotiGameMode::OpenEndScreen,
		3.0f,
		false
	);
}

void ABotiGameMode::OpenEndScreen()
{
	UGameplayStatics::OpenLevel(this, TEXT("L_EndScreen"));
}
