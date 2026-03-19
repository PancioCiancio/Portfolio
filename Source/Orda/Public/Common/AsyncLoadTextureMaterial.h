#pragma once

#include "CoreMinimal.h"
#include "UObject/NameTypes.h"
#include "UObject/SoftObjectPtr.h"
#include "Templates/SharedPointer.h"
#include "Engine/StreamableManager.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "AsyncLoadTextureMaterial.generated.h"


UCLASS(BlueprintType)
class UAsyncLoadTextureMaterial : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	// Asynchronously loads a soft texture and applies it to a Material Instance Dynamic.
	// @param Material		The MID to apply the texture to.
	// @param ParameterName The name of the texture parameter in the material.
	// @param SoftTexture	The soft pointer to the texture asset.
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Materials|Async"))
	static UAsyncLoadTextureMaterial* AsyncLoadTextureMaterial(UMaterialInstanceDynamic* Material, FName ParameterName, TSoftObjectPtr<UTexture> SoftTexture);

	virtual void Activate() override;

private:

	// The callback when the asset manager finishes loading
	void OnLoadCompleted();

private:
	// Mark with UPROPERTY so it get garbage collected by unreal engine
	UPROPERTY()
	UMaterialInstanceDynamic* TargetMaterial;

	FName TargetParameterName;						// Cached parameter to read after async load
	TSoftObjectPtr<UTexture> TargetSoftTexture;		// Cached parameter to read after async load
	
	TSharedPtr<FStreamableHandle> StreamingHandle;
};