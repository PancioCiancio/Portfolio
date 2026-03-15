#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Misc/TextFilter.h"
#include "Templates/SharedPointer.h"

#include "ItemsViewer.generated.h"


// Forward declarations
class UEditableText;
class UTileView;

class UItemDescriptor;
class UItemView;

UCLASS(Blueprintable, BlueprintType)
class UItemsViewer : public UUserWidget
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

	TSharedPtr<TTextFilter<UItemDescriptor*>> SearchBarTextFilter;

// Runtime configuration
protected:

	UFUNCTION(BlueprintCallable)
	void UpdateTileViewer();

private:

	UFUNCTION()
	void OnTextChanged(const FText& Text);

	UFUNCTION()
	void ItemToStringArray(UItemDescriptor* Desc, TArray<FString>& OutFilterStrings);

	UPROPERTY(Transient)
	TArray<UItemDescriptor*> Descs;

	UPROPERTY(Transient)
	TArray<UItemDescriptor*> DescsFiltered;

// Widget references
protected:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableText> EditableText_SearchBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTileView> TileView_Items;
};
