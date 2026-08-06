#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/AC_Posture.h"
#include "Game/BotiGameMode.h"
#include "WBPBossBar.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class BOTI_API UWBPBossBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "HUD")
	ACharacter* OwningCharacter;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* BossNameText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* BossPostureBar;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void BindToBoss(AActor* BossActor, const FText& BossName);

	UFUNCTION(BlueprintPure, Category = "HUD")
	float GetBossPosturePercent() const;

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	UAC_Posture* BossPostureComp;

	UFUNCTION()
	void HandlePhaseChanged(EGamePhase NewPhase);
};
