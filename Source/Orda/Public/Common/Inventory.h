#pragma once

#include "CoreMinimal.h"
#include "Misc/TextFilter.h"
#include "Components/Widget.h"
#include "Containers/Array.h"
#include "Blueprint/UserWidget.h"
#include "Templates/SharedPointer.h"

#include "Inventory.generated.h"

class UTileView;
class UEditableText;

UCLASS(Blueprintable)
class ORDA_API UInvetory : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

protected:

    UPROPERTY(EditAnywhere, Category = "Configuration")
    TSubclassOf<UUserWidget> InventorySlotClass;

    UPROPERTY(BlueprintReadWrite)
    TArray<FString> MasterList;

    UPROPERTY(Transient)
    TArray<FString> FilteredList;

    // 1. Create a convenient alias for your specific filter type
    using FItemTextFilter = TTextFilter<FString>;

    // 2. The actual filter object
    TSharedPtr<FItemTextFilter> SearchFilter;

    // 3. Delegate function to extract strings from an item
    void HandleItemToStringArray(FString Text, TArray<FString>& OutFilterStrings);

    UFUNCTION()
    void OnSearchTextChanged(const FText& Text);

private:

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UEditableText> EditableText_SearchBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTileView> TileView_Inventory;
};