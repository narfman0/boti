#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/AC_Health.h"
#include "WBPDownedOverlay.generated.h"

class UTextBlock;

UCLASS()
class BOTI_API UWBPDownedOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "HUD")
	ACharacter* OwningCharacter;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* CountdownText;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void BindToHealthComponent(UAC_Health* Health);

	// Returns remaining downed seconds formatted as "X.X" for the countdown label.
	UFUNCTION(BlueprintPure, Category = "HUD")
	FText GetCountdownText() const;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY()
	UAC_Health* HealthComp;
};
