#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UObject/SoftObjectPtr.h"
#include "Containers/Array.h"

#include "Inventory/Data/ItemData.h"

#include "InventorySettings.generated.h"


// Global settings for the Inventory System.
// Appears in Edit -> Project Settings -> Game -> Inventory Settings
UCLASS(Config=Game, defaultconfig, meta=(DisplayName="Inventory Settings"))
class UInventorySettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	// The list of Primary Asset Types our Subsystem should load on startup
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Asset Loading")
	TArray<TSoftObjectPtr<UItemData>> DataAssetToLoad;
};