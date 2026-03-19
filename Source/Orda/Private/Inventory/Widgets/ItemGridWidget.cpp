#include "Inventory/Widgets/ItemGridWidget.h"

#include "Engine/AssetManager.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
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
	TArray<FPrimaryAssetId> Ids = {};

	Ids.Reserve(64);
	Descs.Reserve(64);

	const TArray<FPrimaryAssetType, TInlineAllocator<8>> AssetTypes = {
		FPrimaryAssetType(FName(TEXT("Potion"))),
		FPrimaryAssetType(FName(TEXT("Monster")))};

	for (const FPrimaryAssetType& AssetType : AssetTypes)
	{
		AssetManager.GetPrimaryAssetIdList(AssetType, Ids);
	}

	StreamAssetsHandle = AssetManager.LoadPrimaryAssets(Ids, {}, FStreamableDelegate::CreateLambda([=, this]()
		{
			UAssetManager& Manager = UAssetManager::Get();
			FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

			for (const FPrimaryAssetId& Id : Ids)
			{
				UItemDefinition* LoadedDesc = Manager.GetPrimaryAssetObject<UItemDefinition>(Id);
				check(LoadedDesc);

				Descs.Add(LoadedDesc);
			}

			OnTextChanged(FText::GetEmpty());
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

void UItemGridWidget::NativeDestruct()
{
	StreamIconsHandle.Reset();
	StreamAssetsHandle.Reset();
	
	Super::NativeDestruct();
}

void UItemGridWidget::SetSearchedAssetType(FPrimaryAssetType Type)
{
	SearchedAssetType = Type;

	OnTextChanged(EditableText_SearchBar->GetText());
}

void UItemGridWidget::OnTextChanged(const FText& Text)
{
	const TArray<UItemDefinition*> TempDescs = Descs.FilterByPredicate([this](const UItemDefinition* A){
		return !SearchedAssetType.IsValid() || A->GetPrimaryAssetId().PrimaryAssetType == SearchedAssetType;
	});

	TArray<UItemDefinition*> DescsFiltered = {};
	
	Searcher::Filter(SearchBarTextFilter, Text, TempDescs, DescsFiltered);

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

		// You can add more "rules" here...
		// E.g. OutFilterStrings.Add(Desc->Description.ToString());
	}
}
