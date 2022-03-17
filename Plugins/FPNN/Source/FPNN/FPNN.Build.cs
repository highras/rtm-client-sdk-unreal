// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class FPNN : ModuleRules
{
	public FPNN(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnableUndefinedIdentifierWarnings = false;
        bEnableExceptions = true;
        PublicDefinitions.Add("MSGPACK_DISABLE_LEGACY_NIL");

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Library", "x64", "fpnn.lib"));

            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "x64"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "x64", "base"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "x64", "proto"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "x64", "proto", "msgpack"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "x64", "proto", "rapidjson"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "x64", "core"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Library", "MacOS", "libfpnn.a"));

            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "MacOS"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "MacOS", "base"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "MacOS", "proto"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "MacOS", "proto", "msgpack"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "MacOS", "proto", "rapidjson"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "MacOS", "core"));
        }
        else if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Library", "iOS", "libfpnn.a"));

            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "iOS"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "iOS", "base"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "iOS", "proto"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "iOS", "proto", "msgpack"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "iOS", "proto", "rapidjson"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "iOS", "core"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            // select required architecture
            string Architecture = "armeabi-v7a";
            //string Architecture = "arm64-v8a";
            //string Architecture = "x86";
            //string Architecture = "x86_64";

            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Library", "Android", Architecture, "libfpnn.a"));

            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "Android"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "Android", "base"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "Android", "proto"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "Android", "proto", "msgpack"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "Android", "proto", "rapidjson"));
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include", "Android", "core"));
        }

//        PublicIncludePaths.AddRange(
//			new string[] {
//				// ... add public include paths required here ...
//			}
//			);
//				
//		
//		PrivateIncludePaths.AddRange(
//			new string[] {
//				// ... add other private include paths required here ...
//			}
//			);
			
		
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
	}
}
