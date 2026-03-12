using UnrealBuildTool;

public class OrdaEditorTarget : TargetRules
{
    public OrdaEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBUildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.AddRange( new string[] { "OrdaCore" } );
    }
}