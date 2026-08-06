#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BotiHUD.generated.h"

UCLASS()
class BOTI_API UBotiHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	// The character this HUD belongs to — set from Blueprint or owning controller.
	UPROPERTY(BlueprintReadWrite, Category = "HUD")
	ACharacter* OwningCharacter;
};
