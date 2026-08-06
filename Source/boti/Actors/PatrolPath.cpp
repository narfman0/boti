#include "Actors/PatrolPath.h"
#include "Components/SplineComponent.h"

APatrolPath::APatrolPath()
{
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	RootComponent = Spline;
}

FVector APatrolPath::GetPatrolPoint(int32 Index) const
{
	if (!Spline) return GetActorLocation();
	return Spline->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);
}

int32 APatrolPath::GetNumPoints() const
{
	if (!Spline) return 0;
	return Spline->GetNumberOfSplinePoints();
}
