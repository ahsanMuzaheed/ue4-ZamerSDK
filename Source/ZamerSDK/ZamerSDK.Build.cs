// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class ZamerSDK : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }
    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }

	public ZamerSDK(TargetInfo Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"ZamerSDK/Public"
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"ZamerSDK/Private",
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
		LoadZamerSDKLibrary(Target);

	}

	public bool LoadZamerSDKLibrary(TargetInfo Target)
    {
        bool isLibrarySupported = false;

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;
            string platformString = Target.Platform == UnrealTargetPlatform.Win64 ? "Win64" : "Win32";
            platformString = Path.Combine("Libraries", platformString);
            string LibrariesPath = Path.Combine(ThirdPartyPath, platformString);

            PublicLibraryPaths.Add(LibrariesPath);
            PublicDelayLoadDLLs.Add("ZamerSDK_CheckToken.dll");
            RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(LibrariesPath, "ZamerSDK_CheckToken.dll")));
            isLibrarySupported = true;
        }

        return isLibrarySupported;
    }
}
