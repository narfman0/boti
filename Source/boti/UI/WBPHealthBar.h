#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/AC_Health.h"
#include "WBPHealthBar.generated.h"

class UProgressBar;
class UOverlay;

UCLASS()
class BOTI_API UWBPHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "HUD")
	ACharacter* OwningCharacter;

	// Bind these in the WBP Blueprint graph.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* HealthProgressBar;

	// Vignette overlay shown while downed.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UOverlay* DownedVignetteOverlay;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void BindToHealthComponent(UAC_Health* Health);

	// Blueprint-callable percent for easy bar binding.
	UFUNCTION(BlueprintPure, Category = "HUD")
	float GetHealthPercent() const;

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	UAC_Health* HealthComp;

	UFUNCTION()
	void HandleDowned();

	UFUNCTION()
	void HandleRevived();
};
