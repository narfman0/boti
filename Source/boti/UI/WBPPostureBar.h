#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/AC_Posture.h"
#include "WBPPostureBar.generated.h"

class UProgressBar;

UCLASS()
class BOTI_API UWBPPostureBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "HUD")
	ACharacter* OwningCharacter;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* PostureProgressBar;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void BindToPostureComponent(UAC_Posture* Posture);

	UFUNCTION(BlueprintPure, Category = "HUD")
	float GetPosturePercent() const;

	// True when posture < 25% — drives red pulse animation in Blueprint.
	UFUNCTION(BlueprintPure, Category = "HUD")
	bool IsPostureCritical() const;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY()
	UAC_Posture* PostureComp;
};
