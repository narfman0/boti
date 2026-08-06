#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BotiGameMode.generated.h"

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	Exploring    UMETA(DisplayName = "Exploring"),
	InCombat     UMETA(DisplayName = "InCombat"),
	BossPhase2   UMETA(DisplayName = "BossPhase2"),
	BossEscape   UMETA(DisplayName = "BossEscape"),
	Victory      UMETA(DisplayName = "Victory"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseChanged, EGamePhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVictoryDelegate);

UCLASS()
class BOTI_API ABotiGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABotiGameMode();

	UPROPERTY(BlueprintReadOnly, Category = "GameMode")
	EGamePhase CurrentPhase;

	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FOnPhaseChanged OnPhaseChanged;

	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FOnVictoryDelegate OnVictory;

	// Called by AEnemyAnotsu when HP drops below 40%
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void StartBossPhase2();

	// Called when Anotsu's escape BT task fires; starts win sequence
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void TriggerBossEscape();

	// Disables player input, waits 3s, opens L_EndScreen
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void HandleVictory();

protected:
	virtual void BeginPlay() override;

private:
	void SetPhase(EGamePhase NewPhase);
	void OpenEndScreen();

	FTimerHandle VictoryTimerHandle;
};
