using UnrealBuildTool;

public class TPSTeamProjectEditor: ModuleRules
{
    public TPSTeamProjectEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine"});

        PublicDependencyModuleNames.AddRange(
        new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "UnrealEd",
            "Slate",
            "SlateCore",
            "AssetTools",
            "DeveloperSettings",
            "Json",
            "JsonUtilities",
            "Blutility",
            "EditorSubsystem",
            "EditorWidgets",
            "Projects",
            "DesktopWidgets",
            "EditorScriptingUtilities",
            "ToolMenus",
            "AssetRegistry",
            "UMG"
        }
        );
    }
}
