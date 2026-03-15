#include "Inventory/Widgets/ItemGridWidget.h"

#include "Engine/AssetManager.h"
#include "Components/EditableText.h"
#include "Components/TileView.h"
#include "Components/Image.h"

#include "Common/Searcher.h"
#include "Inventory/Data/ItemDefinition.h"
#include "Inventory/Widgets/ItemEntryWidget.h"

void UItemGridWidget::NativeOnInitialized()
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
				UItemDefinition* LoadedDesc = Manager.GetPrimaryAssetObject<UItemDefinition>(Id);
				if (LoadedDesc)
				{
					// 'Descs' MUST be a member variable of the class (e.g., TArray<UItemDescriptor*> Descs)
					Descs.Add(LoadedDesc);
					LoadedDesc->Icon.LoadSynchronous();
				}
			}
		}));


	// Create TTextFilter
	SearchBarTextFilter = MakeShared<TTextFilter<UItemDefinition*>>(
		TTextFilter<UItemDefinition*>::FItemToStringArray::CreateUObject(this, &UItemGridWidget::ItemToStringArray));


	// Bind callback to search bar
	EditableText_SearchBar->OnTextChanged.AddUniqueDynamic(this, &UItemGridWidget::OnTextChanged);
}

void UItemGridWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UItemGridWidget::OnTextChanged(const FText& Text)
{
	DescsFiltered.Empty();

	Searcher::Filter(SearchBarTextFilter, Text, Descs, DescsFiltered);

	TileView_Items->ClearListItems();

	for (int32 i = 0; i < DescsFiltered.Num(); i++)
	{
		TileView_Items->AddItem(DescsFiltered[i]);
	}
}

void UItemGridWidget::ItemToStringArray(UItemDefinition* Desc, TArray<FString>& OutFilterStrings)
{
	if (Desc)
	{
		OutFilterStrings.Add(Desc->Name.ToString());
		//OutFilterStrings.Add(Desc->Description.ToString());
	}
}