// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class RTM : ModuleRules
{
	public RTM(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnableUndefinedIdentifierWarnings = false;
        bEnableExceptions = true; 

		PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private", "AudioConvert", "Include", "opencore-amr", "include"));
		PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private", "AudioConvert", "Include", "vo-amrwbenc", "include"));
		PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private", "AudioConvert", "Include"));

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Private", "AudioConvert", "Library", "x64", "audio-convert.lib"));
            PrivateDependencyModuleNames.Add("AudioCaptureRtAudio");
        }
		else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Private", "AudioConvert", "Library", "MacOS", "libaudio-convert.a"));
            // PrivateDependencyModuleNames.Add("AudioCaptureRtAudio");
			PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Private", "RTMNative", "Include", "MacOS"));
			PublicFrameworks.Add(Path.Combine(ModuleDirectory, "Private", "RTMNative", "Library", "MacOS", "RTMNative.framework"));
		}
		else if (Target.Platform == UnrealTargetPlatform.IOS)
		{
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Private", "AudioConvert", "Library", "iOS", "libaudio-convert.a"));
            PrivateDependencyModuleNames.Add("AudioCaptureAudioUnit");

            PublicAdditionalFrameworks.Add( new Framework( "RTMNative", Path.Combine(ModuleDirectory, "Private", "RTMNative", "Library", "iOS", "RTMNative.framework.zip"), "", true) );
			PublicFrameworks.AddRange(
               		new string[] {
						"Foundation",
						"AVFoundation",
               			"CoreTelephony",
               			"Security",
               			"SystemConfiguration",
               			"AudioToolbox",
               			"CoreAudio",
				}
			);
		}
		else if (Target.Platform == UnrealTargetPlatform.Android)
        {
			PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });
	        string aplPath = Path.Combine(ModuleDirectory, "Private", "RTMNative", "Library", "Android", "RTM_APL.xml");
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", aplPath); 

            // select required architecture
            string Architecture = "armeabi-v7a";
            //string Architecture = "arm64-v8a";
            //string Architecture = "x86";
            //string Architecture = "x86_64";

            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Private", "AudioConvert", "Library", "Android", Architecture, "libaudio-convert.a"));
            //PrivateDependencyModuleNames.Add("AudioCaptureAndroid");
        }
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
				"FPNN",
				"AudioCaptureCore",
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
				"AudioMixer",
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
