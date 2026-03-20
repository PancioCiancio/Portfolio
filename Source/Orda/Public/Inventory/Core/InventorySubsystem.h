#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Templates/SharedPointer.h"
#include "Engine/StreamableManager.h"

#include "InventorySubsystem.generated.h"

// Forward declaration
class UItemData;


UCLASS()
class UInventorySubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable)
    const TArray<UItemData*>& GetLoadedItems() const;

private:

    UPROPERTY(Transient)
    TArray<UItemData*> LoadedItems;

	TSharedPtr<FStreamableHandle> StreamAssetsHandle;
};