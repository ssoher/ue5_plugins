namespace UnrealBuildTool.Rules
{
	public class SublimeTextSourceCodeAccess : ModuleRules
	{
		public SublimeTextSourceCodeAccess(ReadOnlyTargetRules Target) : base(Target)
		{
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"DesktopPlatform",
					"SourceCodeAccess"
				}
			);

			ShortName = "STSCA";
		}
	}
}
