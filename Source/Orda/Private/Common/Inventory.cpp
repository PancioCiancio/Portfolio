#include "Common/Inventory.h"

#include "Components/EditableText.h"
#include "Components/TileView.h"

#include "Common/Searcher.h"

void UInvetory::NativeConstruct()
{
	Super::NativeConstruct();

    // 1. Initialize the filter and bind it to your extraction delegate
    SearchFilter = MakeShared<FItemTextFilter>(
        FItemTextFilter::FItemToStringArray::CreateUObject(this, &UInvetory::HandleItemToStringArray)
    );

    EditableText_SearchBar->OnTextChanged.AddUniqueDynamic(this, &UInvetory::OnSearchTextChanged);
}

// 2. This function feeds searchable data to the token evaluator
void UInvetory::HandleItemToStringArray(FString Text, TArray<FString>& OutFilterStrings)
{
    // Add EVERYTHING you want the player to be able to search for
    OutFilterStrings.Add(Text);
}

// 3. Filtering the actual UI
void UInvetory::OnSearchTextChanged(const FText& Text)
{
    // Filter the list of items.
    Searcher::Filter(SearchFilter, Text, MasterList, FilteredList);

    // Now update your UListView or visual layout
    //RefreshInventoryUI();
    TileView_Inventory->ClearListItems();
}