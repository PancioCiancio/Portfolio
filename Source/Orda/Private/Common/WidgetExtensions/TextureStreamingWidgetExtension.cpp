#include "Common/WidgetExtensions/TextureStreamingWidgetExtension.h"

#include "Engine/AssetManager.h"

void UTextureStreamingWidgetExtension::RequestTexture(UMaterialInstanceDynamic* Material, FName Parameter, TSoftObjectPtr<UTexture2D> SoftTexture)
{
    Release(); // cancel any previous in-flight load

    if (!Material || SoftTexture.IsNull()) return;

    PendingMaterial = Material;
    PendingParameter = Parameter;
    PendingTexture = SoftTexture;

    // Fast path
    if (UTexture* Already = SoftTexture.Get())
    {
        Material->SetTextureParameterValue(Parameter, Already);
        return;
    }

    // Async path
    StreamingHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
        SoftTexture.ToSoftObjectPath(),
        [this]()
        {
            if (!StreamingHandle.IsValid()) return; // was cancelled
            if (PendingMaterial)
                if (UTexture* Loaded = PendingTexture.Get())
                    PendingMaterial->SetTextureParameterValue(PendingParameter, Loaded);
        },
        FStreamableManager::AsyncLoadHighPriority
    );
}

void UTextureStreamingWidgetExtension::Release()
{
    if (StreamingHandle.IsValid())
    {
        StreamingHandle->CancelHandle();
        StreamingHandle.Reset();
    }

    // Should the texture be unloaded?
    // I think it should be in charge of the garbage collector.
}

void UTextureStreamingWidgetExtension::Destruct()
{
    // Widget is being torn down — release automatically, no manual cleanup needed
    Release();

    Super::Destruct();
}
