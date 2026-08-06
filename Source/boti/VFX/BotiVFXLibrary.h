#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "BotiVFXLibrary.generated.h"

class UNiagaraSystem;
class UMaterialInterface;

UCLASS()
class BOTI_API UBotiVFXLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Assign these in Project Settings > Default Classes or via a Data Asset.
	// Accessed via GetDefault<UBotiVFXLibrary>() from the static functions.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	TSoftObjectPtr<UNiagaraSystem> ParrySparkSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	TSoftObjectPtr<UNiagaraSystem> BloodwormSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	TSoftObjectPtr<UMaterialInterface> HitDecalMaterial;

	UFUNCTION(BlueprintCallable, Category = "VFX", meta = (WorldContext = "WorldCtx"))
	static void SpawnParrySpark(UObject* WorldCtx, FVector Location);

	UFUNCTION(BlueprintCallable, Category = "VFX", meta = (WorldContext = "WorldCtx"))
	static void SpawnBloodworm(UObject* WorldCtx, FVector Location);

	UFUNCTION(BlueprintCallable, Category = "VFX", meta = (WorldContext = "WorldCtx"))
	static void SpawnHitDecal(UObject* WorldCtx, FVector Location, FRotator Rotation);
};
