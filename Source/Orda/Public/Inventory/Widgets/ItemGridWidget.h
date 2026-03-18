#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Misc/TextFilter.h"
#include "Templates/SharedPointer.h"
#include "Engine/StreamableManager.h"

#include "ItemGridWidget.generated.h"


// Forward declarations
class UEditableText;
class UTileView;
class UTextBlock;

class UItemDefinition;
class UItemEntryWidget;

UCLASS(Blueprintable, BlueprintType)
class UItemGridWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

// Configuration
protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ItemVisualClass;

private:

	TSharedPtr<TTextFilter<UItemDefinition*>> SearchBarTextFilter;

// Runtime configuration
public:

	UFUNCTION(BlueprintCallable)
	void SetSearchedAssetType(FPrimaryAssetType Type);

private:

	UFUNCTION()
	void OnTextChanged(const FText& Text);

	UFUNCTION()
	void ItemToStringArray(UItemDefinition* Desc, TArray<FString>& OutFilterStrings);

	UPROPERTY(Transient)
	TArray<UItemDefinition*> Descs;

	UPROPERTY(Transient)
	FPrimaryAssetType SearchedAssetType;

	TSharedPtr<FStreamableHandle> StreamAssetsHandle;
	TSharedPtr<FStreamableHandle> StreamIconsHandle;

// Widget references
protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableText> EditableText_SearchBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTileView> TileView_Items;
};
