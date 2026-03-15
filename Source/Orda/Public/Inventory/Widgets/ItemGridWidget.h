#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Misc/TextFilter.h"
#include "Templates/SharedPointer.h"

#include "ItemGridWidget.generated.h"


// Forward declarations
class UEditableText;
class UTileView;

class UItemDefinition;
class UItemEntryWidget;

UCLASS(Blueprintable, BlueprintType)
class UItemGridWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

// Configuration
protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ItemVisualClass;

private:

	UPROPERTY(Transient)
	TArray<FPrimaryAssetId> Ids;

	TSharedPtr<TTextFilter<UItemDefinition*>> SearchBarTextFilter;

// Runtime configuration
private:

	UFUNCTION()
	void OnTextChanged(const FText& Text);

	UFUNCTION()
	void ItemToStringArray(UItemDefinition* Desc, TArray<FString>& OutFilterStrings);

	UPROPERTY(Transient)
	TArray<UItemDefinition*> Descs;

	UPROPERTY(Transient)
	TArray<UItemDefinition*> DescsFiltered;

// Widget references
protected:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableText> EditableText_SearchBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTileView> TileView_Items;
};
