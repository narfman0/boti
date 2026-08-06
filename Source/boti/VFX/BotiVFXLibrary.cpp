#include "VFX/BotiVFXLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBotiVFXLibrary::SpawnParrySpark(UObject* WorldCtx, FVector Location)
{
	const UBotiVFXLibrary* CDO = GetDefault<UBotiVFXLibrary>();
	UNiagaraSystem* System = CDO->ParrySparkSystem.LoadSynchronous();
	if (System && WorldCtx)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(WorldCtx, System, Location);
	}
}

void UBotiVFXLibrary::SpawnBloodworm(UObject* WorldCtx, FVector Location)
{
	const UBotiVFXLibrary* CDO = GetDefault<UBotiVFXLibrary>();
	UNiagaraSystem* System = CDO->BloodwormSystem.LoadSynchronous();
	if (System && WorldCtx)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(WorldCtx, System, Location);
	}
}

void UBotiVFXLibrary::SpawnHitDecal(UObject* WorldCtx, FVector Location, FRotator Rotation)
{
	const UBotiVFXLibrary* CDO = GetDefault<UBotiVFXLibrary>();
	UMaterialInterface* Mat = CDO->HitDecalMaterial.LoadSynchronous();
	if (Mat && WorldCtx)
	{
		UGameplayStatics::SpawnDecalAtLocation(
			WorldCtx, Mat, FVector(32.f, 32.f, 32.f), Location, Rotation, 10.f);
	}
}
