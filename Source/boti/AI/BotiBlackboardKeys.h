#pragma once

#include "CoreMinimal.h"

// Canonical blackboard key names for all boti enemy behavior trees.
// Create a BB_Enemy asset in the editor with these exact key names:
//   TargetActor     Object/Actor  — the player
//   bTargetDowned   Bool          — player is currently downed
//   PatrolIndex     Int           — current patrol waypoint index
//   ComboHitCount   Int           — hits landed in the current attack combo
//   AnotsuPhase     Int           — Anotsu current boss phase (1 or 2)
namespace BBKeys
{
	static const FName TargetActor   { TEXT("TargetActor")   };
	static const FName bTargetDowned { TEXT("bTargetDowned") };
	static const FName PatrolIndex   { TEXT("PatrolIndex")   };
	static const FName ComboHitCount { TEXT("ComboHitCount") };
	static const FName AnotsuPhase   { TEXT("AnotsuPhase")   };
}
