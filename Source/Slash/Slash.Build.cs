// copyright: none

using UnrealBuildTool;

public class Slash : ModuleRules
{
	public Slash(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","EnhancedInput","HairStrandsCore","Niagara", "GeometryCollectionEngine" });

		// Workaround for UE 5.4 compilation error with newer MSVC versions
		PublicDefinitions.Add("PLATFORM_HAS_ASAN_INCLUDE=0");

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
