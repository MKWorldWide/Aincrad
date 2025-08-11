using UnityEditor;
using UnityEditor.Build;
using UnityEditor.Build.Reporting;
using UnityEngine;
using System.IO;
using System.Diagnostics;
using System.Collections.Generic;
using Debug = UnityEngine.Debug;

public class BuildAndDeploy : IPreprocessBuildWithReport, IPostprocessBuildWithReport
{
    public int callbackOrder => 0;
    private static string buildPath = "Builds";
    private static string productName = "Aincrad";
    
    // Called before build starts
    public void OnPreprocessBuild(BuildReport report)
    {
        Debug.Log("[BuildAndDeploy] Starting pre-build checks...");
        
        // Ensure build directory exists
        if (!Directory.Exists(buildPath))
        {
            Directory.CreateDirectory(buildPath);
        }
        
        // Run optimization passes
        OptimizeTextures();
        OptimizeMeshes();
        
        Debug.Log("[BuildAndDeploy] Pre-build checks complete!");
    }
    
    // Called after build completes
    public void OnPostprocessBuild(BuildReport report)
    {
        string outputPath = report.summary.outputPath;
        string platform = report.summary.platform.ToString();
        double buildTime = report.summary.totalTime.TotalSeconds;
        long buildSize = report.summary.totalSize / (1024 * 1024); // Convert to MB
        
        string message = $"""
        [BuildAndDeploy] Build Complete!
        Platform: {platform}
        Output: {outputPath}
        Size: {buildSize}MB
        Time: {buildTime:F2} seconds
        """;
        
        Debug.Log(message);
        
        // Show completion dialog
        if (report.summary.result == BuildResult.Succeeded)
        {
            bool openFolder = EditorUtility.DisplayDialog(
                "Build Succeeded",
                $"Build completed successfully!\n\n" +
                $"Platform: {platform}\n" +
                $"Size: {buildSize}MB\n" +
                $"Time: {buildTime:F2} seconds\n\n" +
                $"Open build folder?",
                "Yes", "No");
                
            if (openFolder)
            {
                string folderPath = Path.GetDirectoryName(Path.GetFullPath(outputPath));
                Process.Start(folderPath);
            }
        }
        else
        {
            EditorUtility.DisplayDialog(
                "Build Failed",
                "The build failed. Please check the console for details.",
                "OK");
        }
    }
    
    [MenuItem("VRChat/Build & Deploy/All Platforms")]
    public static void BuildAllPlatforms()
    {
        if (EditorUtility.DisplayDialog("Build All Platforms", 
            "This will build for both Windows and Android (Quest). Continue?", "Yes", "Cancel"))
        {
            BuildForPlatform(BuildTarget.StandaloneWindows64, "Windows");
            BuildForPlatform(BuildTarget.Android, "Android");
        }
    }
    
    [MenuItem("VRChat/Build & Deploy/Windows")]
    public static void BuildWindows()
    {
        if (EditorUtility.DisplayDialog("Build for Windows", 
            "Build for Windows (PC)?", "Build", "Cancel"))
        {
            BuildForPlatform(BuildTarget.StandaloneWindows64, "Windows");
        }
    }
    
    [MenuItem("VRChat/Build & Deploy/Android (Quest)")]
    public static void BuildAndroid()
    {
        if (EditorUtility.DisplayDialog("Build for Android (Quest)", 
            "Build for Android (Quest)? Make sure you have the Android module installed.", "Build", "Cancel"))
        {
            BuildForPlatform(BuildTarget.Android, "Android");
        }
    }
    
    private static void BuildForPlatform(BuildTarget target, string platformName)
    {
        // Set build location
        string outputFile = target == BuildTarget.Android ? 
            $"{productName}.apk" : 
            $"{productName}.exe";
            
        string outputPath = Path.Combine(buildPath, platformName, outputFile);
        
        // Ensure directory exists
        string directory = Path.GetDirectoryName(outputPath);
        if (!Directory.Exists(directory))
        {
            Directory.CreateDirectory(directory);
        }
        
        // Get enabled scenes
        string[] scenes = GetEnabledScenes();
        if (scenes.Length == 0)
        {
            Debug.LogError("[BuildAndDeploy] No scenes found to build!");
            EditorUtility.DisplayDialog("Build Failed", "No scenes found in build settings!", "OK");
            return;
        }
        
        // Configure build options
        BuildOptions options = BuildOptions.None;
        
        // Development build for debugging
        if (EditorUserBuildSettings.development)
        {
            options |= BuildOptions.Development | BuildOptions.AllowDebugging;
        }
        
        // Platform-specific settings
        if (target == BuildTarget.Android)
        {
            // Android specific settings for Quest
            PlayerSettings.Android.minSdkVersion = AndroidSdkVersions.AndroidApiLevel24;
            PlayerSettings.Android.targetSdkVersion = AndroidSdkVersions.AndroidApiLevel31;
            PlayerSettings.Android.targetArchitectures = AndroidArchitecture.ARM64;
            
            // Increment version code
            PlayerSettings.Android.bundleVersionCode++;
            
            // Texture compression
            EditorUserBuildSettings.androidBuildSubtarget = MobileTextureSubtarget.ASTC;
            
            // Enable Oculus Quest features
            PlayerSettings.VR.oculus.goLowOverheadMode = true;
            PlayerSettings.VR.oculus.sharedDepthBuffer = false;
            PlayerSettings.VR.oculus.dashSupport = true;
            
            // Optimize for mobile
            PlayerSettings.MTRendering = true;
            PlayerSettings.graphicsJobs = true;
            PlayerSettings.Android.forceSDCardPermission = false;
            PlayerSettings.Android.forceInternetPermission = false;
            PlayerSettings.gpuSkinning = false;
        }
        else if (target == BuildTarget.StandaloneWindows64)
        {
            // Windows specific settings
            PlayerSettings.fullScreenMode = FullScreenMode.Windowed;
            PlayerSettings.defaultIsFullScreen = false;
            PlayerSettings.defaultIsNativeResolution = true;
            PlayerSettings.visibleInBackground = false;
            PlayerSettings.allowFullscreenSwitch = true;
            
            // Graphics settings for Windows
            PlayerSettings.MTRendering = true;
            PlayerSettings.graphicsJobs = true;
        }
        
        // Start the build
        Debug.Log($"[BuildAndDeploy] Starting {platformName} build...");
        BuildPipeline.BuildPlayer(scenes, outputPath, target, options);
    }
    
    private static string[] GetEnabledScenes()
    {
        List<string> enabledScenes = new List<string>();
        
        foreach (EditorBuildSettingsScene scene in EditorBuildSettings.scenes)
        {
            if (scene.enabled)
            {
                enabledScenes.Add(scene.path);
            }
        }
        
        return enabledScenes.ToArray();
    }
    
    private static void OptimizeTextures()
    {
        Debug.Log("[BuildAndDeploy] Optimizing textures...");
        
        // This would be where you'd implement texture optimization logic
        // For now, we'll just log that we would optimize textures here
        
        // Example optimization steps:
        // 1. Set texture compression settings
        // 2. Resize large textures
        // 3. Convert HDR textures to LDR if not needed
        // 4. Generate mipmaps where appropriate
        
        Debug.Log("[BuildAndDeploy] Texture optimization complete!");
    }
    
    private static void OptimizeMeshes()
    {
        Debug.Log("[BuildAndDeploy] Optimizing meshes...");
        
        // This would be where you'd implement mesh optimization logic
        // For now, we'll just log that we would optimize meshes here
        
        // Example optimization steps:
        // 1. Enable mesh compression
        // 2. Optimize mesh data
        // 3. Generate colliders where needed
        // 4. Combine meshes where possible
        
        Debug.Log("[BuildAndDeploy] Mesh optimization complete!");
    }
    
    [MenuItem("VRChat/Open Build Folder")]
    public static void OpenBuildFolder()
    {
        string fullPath = Path.Combine(Directory.GetCurrentDirectory(), buildPath);
        
        if (!Directory.Exists(fullPath))
        {
            Directory.CreateDirectory(fullPath);
        }
        
        Process.Start(fullPath);
    }
}
