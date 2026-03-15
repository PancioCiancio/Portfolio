#pragma once

#include "CoreMinimal.h"
#include "UObject/SoftObjectPtr.h"
#include "Internationalization/Text.h"
#include "Engine/DataAsset.h"

#include "ItemDefinition.generated.h"

class UTexture2D;

UCLASS(Blueprintable, BlueprintType)
class UItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Icon;

	// The designer sets this in the Editor (e.g.: "Weapon", "Potion", "Armor")
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetType Type;

	// Unreal calls this to figure out the item's true Unique ID
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		// We combine the designer's Type with the file's name.
		return FPrimaryAssetId(Type, GetFName());
	}
};
