#include "Common/AsyncLoadTextureMaterial.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Texture.h"
#include "Engine/AssetManager.h"

UAsyncLoadTextureMaterial* UAsyncLoadTextureMaterial::AsyncLoadTextureMaterial(UMaterialInstanceDynamic* Material, FName ParameterName, TSoftObjectPtr<UTexture> SoftTexture)
{
	// Instantiate the async action node
	UAsyncLoadTextureMaterial* Action = NewObject<UAsyncLoadTextureMaterial>();
	Action->TargetMaterial = Material;
	Action->TargetParameterName = ParameterName;
	Action->TargetSoftTexture = SoftTexture;

	// Registering prevents the action from being garbage collected while waiting for the async load
	if (Material)
	{
		Action->RegisterWithGameInstance(Material);
	}

	return Action;
}

void UAsyncLoadTextureMaterial::Activate()
{
	// Validate inputs
	if (!TargetMaterial || TargetSoftTexture.IsNull())
	{
		SetReadyToDestroy();
		return;
	}

	// Fast Path: The texture is already loaded in memory
	if (UTexture* LoadedTexture = TargetSoftTexture.Get())
	{
		TargetMaterial->SetTextureParameterValue(TargetParameterName, LoadedTexture);
		SetReadyToDestroy();
		return;
	}

	// Async Path: Request the load from the Asset Manager
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();

	StreamingHandle = StreamableManager.RequestAsyncLoad(
		TargetSoftTexture.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(this, &UAsyncLoadTextureMaterial::OnLoadCompleted),
		FStreamableManager::AsyncLoadHighPriority // Keep priority high for UI/Visuals
	);
}

void UAsyncLoadTextureMaterial::OnLoadCompleted()
{
	UTexture* LoadedTexture = TargetSoftTexture.Get();

	// Check if material is still valid (the actor/widget might have been destroyed during the load time)
	if (TargetMaterial && LoadedTexture)
	{
		TargetMaterial->SetTextureParameterValue(TargetParameterName, LoadedTexture);
	}

	// Clean up the async action memory
	SetReadyToDestroy();
}
