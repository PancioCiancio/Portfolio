#pragma once

#include "CoreMinimal.h"
#include "UObject/NameTypes.h"
#include "UObject/ObjectPtr.h"
#include "UObject/SoftObjectPtr.h"
#include "Templates/SharedPointer.h"
#include "Engine/StreamableManager.h"
#include "Extensions/UserWidgetExtension.h"

#include "TextureStreamingWidgetExtension.generated.h"


// Forward declaration
class UMaterialInstanceDynamic;
class UTexture2D;


UCLASS(Blueprintable, BlueprintType)
class UTextureStreamingWidgetExtension : public UUserWidgetExtension
{
	GENERATED_BODY()

public:

	// @param Material
	// @param Parameter
	// @param Texture
	UFUNCTION(BlueprintCallable, Category = "Streaming")
	void RequestTexture(UMaterialInstanceDynamic* Material, FName Parameter, TSoftObjectPtr<UTexture2D> SoftTexture);

	//
	UFUNCTION(BlueprintCallable, Category = "Streaming")
	void Release();

	//
	virtual void Destruct() override;

private:

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> PendingMaterial;
	
	TSoftObjectPtr<UTexture2D> PendingTexture;
	
	FName PendingParameter;
	
	TSharedPtr<FStreamableHandle> StreamingHandle;
};