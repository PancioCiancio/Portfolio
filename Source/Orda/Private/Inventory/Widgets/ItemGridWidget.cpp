#include "Inventory/Widgets/ItemGridWidget.h"

#include "Engine/AssetManager.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "Components/TileView.h"
#include "Components/Image.h"

#include "Common/Searcher.h"
#include "Inventory/Data/ItemData.h"
#include "Inventory/Core/InventorySubsystem.h"

void UItemGridWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Create TTextFilter
	SearchBarTextFilter = MakeShared<TTextFilter<UItemData*>>(
		TTextFilter<UItemData*>::FItemToStringArray::CreateUObject(this, &UItemGridWidget::ItemToStringArray));

	// Bind callback to search bar
	EditableText_SearchBar->OnTextChanged.AddUniqueDynamic(this, &UItemGridWidget::OnTextChanged);

	// Init all entries and show them all
	OnTextChanged(FText::GetEmpty());
}

void UItemGridWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UItemGridWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UItemGridWidget::SetSearchedAssetType(FPrimaryAssetType Type)
{
	OnTextChanged(EditableText_SearchBar->GetText());
}

void UItemGridWidget::OnTextChanged(const FText& Text)
{
	const UGameInstance* GameInstance = GetGameInstance();
	check(GameInstance);

	const UInventorySubsystem* Subsystem = GameInstance->GetSubsystem<UInventorySubsystem>();
	check(Subsystem);

	const TArray<UItemData*> TempDescs = Subsystem->GetLoadedItems().FilterByPredicate([this](const UItemData* A) {
		return true;
	});

	TArray<UItemData*> DescsFiltered = {};
	DescsFiltered.Reserve(TempDescs.Num());

	Searcher::Filter(SearchBarTextFilter, Text, TempDescs, DescsFiltered);

	TileView_Items->ClearListItems();

	for (int32 i = 0; i < DescsFiltered.Num(); i++)
	{
		TileView_Items->AddItem(DescsFiltered[i]);
	}
}

void UItemGridWidget::ItemToStringArray(UItemData* Desc, TArray<FString>& OutFilterStrings)
{
	if (Desc)
	{
		OutFilterStrings.Add(Desc->Name.ToString());

		// You can add more "rules" here...
		// E.g. OutFilterStrings.Add(Desc->Description.ToString());
	}
}
