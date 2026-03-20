#include "Inventory/Core/InventorySubsystem.h"

#include "Engine/AssetManager.h"
#include "Inventory/Data/ItemData.h"
#include "Inventory/Core/InventorySettings.h"

void UInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    const UInventorySettings* Settings = GetDefault<UInventorySettings>();
    check(Settings);

    // Do heap allocation ones.
    LoadedItems.Reserve(Settings->DataAssetToLoad.Num());

    // Block the main thread until load all data assets.
    // Avoid complexity for demo.
    for (TSoftObjectPtr<UItemData> Asset : Settings->DataAssetToLoad)
    {
        LoadedItems.Add(Asset.LoadSynchronous());
    }
}

void UInventorySubsystem::Deinitialize()
{
    Super::Deinitialize();
}

const TArray<UItemData*>& UInventorySubsystem::GetLoadedItems() const
{
    return LoadedItems;
}