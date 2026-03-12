#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FDirkCore : public IModuleInterface
{
public:
    static inline FOrdaCore& Get()
    {
        return FModuleManager::LoadModuleChecked<FOrdaCore>("OrdaCore");
    }

    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("OrdaCore");
    }

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
}