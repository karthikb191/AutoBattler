#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameFramework/Actor.h"
#include "Math/Vector2D.h"
#include "Math/IntPoint.h"
#include "Level/Tile.h"
#include "Character/Creature.h"
#include "DABattleInfo.generated.h"

USTRUCT(BlueprintType)
struct FBattleInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACreature>			CreatureA;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACreature>			CreatureB;
};

UCLASS()
class AUTOBATTLER_API UDABattleInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FBattleInfo		BattleInfo;
};