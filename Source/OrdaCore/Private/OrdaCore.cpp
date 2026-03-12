#include "OrdaCore.h"
#include "Modules/ModuleManager.h"

#include "Log.h"

void FOrdaCore::StartupModule()
{
    UE_LOG(LogOrdaCore, Log, TEXT("OrdaCore module starting up"));
}

void FOrdaCore::ShutdownModule()
{
    UE_LOG(LogOrdaCore, Log, TEXT("OrdaCore module shutting down"));
}

IMPLEMENT_PRIMARY_GAME_MODULE(FOrdaCore, OrdaCore, "OrdaCore");