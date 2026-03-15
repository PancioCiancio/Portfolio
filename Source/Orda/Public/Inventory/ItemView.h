#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserListEntry.h"

#include "ItemView.generated.h"

// Forward declaration
class UImage;

UCLASS(Blueprintable, BlueprintType)
class UItemView : public UUserWidget, public IUserListEntry
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;
};