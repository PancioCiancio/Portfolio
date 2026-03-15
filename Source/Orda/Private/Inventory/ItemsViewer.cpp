#include "Inventory/ItemsViewer.h"

#include "Engine/AssetManager.h"
#include "Components/EditableText.h"
#include "Components/TileView.h"
#include "Components/Image.h"

#include "Common/Searcher.h"
#include "Inventory/ItemDescriptor.h"
#include "Inventory/ItemView.h"

void UItemsViewer::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Gather all items
	UAssetManager& AssetManager = UAssetManager::Get();

	Ids.Reserve(64);
	Descs.Reserve(64);
	Descs.Reserve(64);

	const TArray<FPrimaryAssetType, TInlineAllocator<8>> AssetTypes = {
		//FPrimaryAssetType(FName(TEXT("Potion"))),
		//FPrimaryAssetType(FName(TEXT("Armor"))),
		//FPrimaryAssetType(FName(TEXT("Weapon"))),
		FPrimaryAssetType(FName(TEXT("Monster")))};

	for (const FPrimaryAssetType& AssetType : AssetTypes)
	{
		AssetManager.GetPrimaryAssetIdList(AssetType, Ids);
	}

	AssetManager.LoadPrimaryAssets(Ids, {}, FStreamableDelegate::CreateLambda([this]()
		{
			// 3. Always re-fetch the Asset Manager inside async callbacks 
			// to avoid storing references to singletons unnecessarily.
			UAssetManager& Manager = UAssetManager::Get();

			for (const FPrimaryAssetId& Id : Ids)
			{
				UItemDescriptor* LoadedDesc = Manager.GetPrimaryAssetObject<UItemDescriptor>(Id);
				if (LoadedDesc)
				{
					// 'Descs' MUST be a member variable of the class (e.g., TArray<UItemDescriptor*> Descs)
					Descs.Add(LoadedDesc);
					LoadedDesc->Icon.LoadSynchronous();
				}
			}
		}));


	// Create TTextFilter
	SearchBarTextFilter = MakeShared<TTextFilter<UItemDescriptor*>>(
		TTextFilter<UItemDescriptor*>::FItemToStringArray::CreateUObject(this, &UItemsViewer::ItemToStringArray));


	// Bind callback to search bar
	EditableText_SearchBar->OnTextChanged.AddUniqueDynamic(this, &UItemsViewer::OnTextChanged);
}

void UItemsViewer::NativeConstruct()
{
	Super::NativeConstruct();
}

// Called in blueprint when the TileView initialize all new entries
void UItemsViewer::UpdateTileViewer()
{
	for (int32 i = 0; i < DescsFiltered.Num(); i++)
	{
		Cast<UItemView>(TileView_Items->GetItemAt(i))->Image_Icon->SetBrushFromTexture(DescsFiltered[i]->Icon.Get());
	}
}

void UItemsViewer::OnTextChanged(const FText& Text)
{
	DescsFiltered.Empty();

	Searcher::Filter(SearchBarTextFilter, Text, Descs, DescsFiltered);

	TileView_Items->ClearListItems();

	for (int32 i = 0; i < DescsFiltered.Num(); i++)
	{
		TileView_Items->AddItem(DescsFiltered[i]);
	}
}

void UItemsViewer::ItemToStringArray(UItemDescriptor* Desc, TArray<FString>& OutFilterStrings)
{
	if (Desc)
	{
		OutFilterStrings.Add(Desc->Name.ToString());
		//OutFilterStrings.Add(Desc->Description.ToString());
	}
}