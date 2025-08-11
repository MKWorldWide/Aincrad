using UnityEditor;
using UnityEditor.Build;
using UnityEditor.Build.Reporting;
using UnityEngine;
using System.IO;
using System.Diagnostics;
using Debug = UnityEngine.Debug;

public class BuildScript
{
    private const string BuildPath = "Builds";
    private const string ProductName = "Aincrad";
    
    [MenuItem("VRChat/Build/All Platforms")]
    public static void BuildAllPlatforms()
    {
        // Build for Windows
        BuildForPlatform(BuildTarget.StandaloneWindows64, "Windows");
        
        // Build for Android (Quest)
        BuildForPlatform(BuildTarget.Android, "Android");
    }
    
    [MenuItem("VRChat/Build/Windows")]
    public static void BuildWindows()
    {
        BuildForPlatform(BuildTarget.StandaloneWindows64, "Windows");
    }
    
    [MenuItem("VRChat/Build/Android (Quest)")]
    public static void BuildAndroid()
    {
        BuildForPlatform(BuildTarget.Android, "Android");
    }
    
    private static void BuildForPlatform(BuildTarget target, string platformName)
    {
        // Set build location
        string locationPathName = Path.Combine(BuildPath, platformName, 
            target == BuildTarget.Android ? $"{ProductName}.apk" : $"{ProductName}.exe");
        
        // Ensure build directory exists
        string directory = Path.GetDirectoryName(locationPathName);
        if (!Directory.Exists(directory))
        {
            Directory.CreateDirectory(directory);
        }
        
        // Get enabled scenes
        string[] scenes = GetEnabledScenes();
        if (scenes.Length == 0)
        {
            Debug.LogError("No scenes found to build!");
            return;
        }
        
        // Configure build options
        BuildOptions options = BuildOptions.None;
        if (target == BuildTarget.Android)
        {
            // Android specific settings
            PlayerSettings.Android.minSdkVersion = AndroidSdkVersions.AndroidApiLevel24;
            PlayerSettings.Android.targetSdkVersion = AndroidSdkVersions.AndroidApiLevel31;
            PlayerSettings.Android.targetArchitectures = AndroidArchitecture.ARM64;
            PlayerSettings.Android.bundleVersionCode++;
            
            // Texture compression
            EditorUserBuildSettings.androidBuildSubtarget = MobileTextureSubtarget.ASTC;
            
            // Development build for debugging
            if (EditorUserBuildSettings.development)
            {
                options |= BuildOptions.Development | BuildOptions.AllowDebugging;
            }
        }
        
        // Build the player
        BuildReport report = BuildPipeline.BuildPlayer(scenes, locationPathName, target, options);
        BuildSummary summary = report.summary;
        
        if (summary.result == BuildResult.Succeeded)
        {
            string size = (summary.totalSize / (1024f * 1024f)).ToString("F2");
            string message = $"Build succeeded: {size}MB\n" +
                           $"Output: {summary.outputPath}\n" +
                           $"Time: {summary.totalTime.TotalSeconds:F2} seconds";
            
            Debug.Log(message);
            EditorUtility.DisplayDialog("Build Succeeded", message, "OK");
            
            // Open the build folder
            Process.Start(Path.GetDirectoryName(Path.GetFullPath(summary.outputPath)));
        }
        else if (summary.result == BuildResult.Failed)
        {
            string error = "Build failed with " + summary.totalErrors + " errors.\n" +
                         "Check the console for details.";
            
            Debug.LogError(error);
            EditorUtility.DisplayDialog("Build Failed", error, "OK");
        }
    }
    
    private static string[] GetEnabledScenes()
    {
        System.Collections.Generic.List<string> scenes = new System.Collections.Generic.List<string>();
        
        foreach (EditorBuildSettingsScene scene in EditorBuildSettings.scenes)
        {
            if (scene.enabled)
            {
                scenes.Add(scene.path);
            }
        }
        
        return scenes.ToArray();
    }
}
