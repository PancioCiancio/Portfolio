using UnrealBuildTool;

public class OrdaTarget : TargetRules
{
    public OrdaTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBUildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.AddRange( new string[] { "OrdaCore" } );
    }
}