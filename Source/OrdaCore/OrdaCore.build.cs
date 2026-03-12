using UnrealBuildTool;

public class OrdaCore : ModuleRules
{
    public OrdaCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHYsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnforeIWYU = true;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" } );
        PrivateDependencyModuleNames.AddRange(new string[] { "" } );
    }
}