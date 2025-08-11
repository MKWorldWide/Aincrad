using UnityEditor;
using UnityEditor.Build;
using UnityEditor.Build.Reporting;
using UnityEngine;
using System.IO;
using System.Collections.Generic;
using System.Linq;

public class VRBuildAutomation : IPreprocessBuildWithReport, IPostprocessBuildWithReport
{
    public int callbackOrder => 0;
    
    // Build configuration
    private static class BuildConfig
    {
        public const string BuildPath = "Builds";
        public static readonly string[] RequiredScenes = 
        {
            "Assets/Scenes/Main.unity"  // Update this with your actual scene path
        };
    }
    
    // Called before the build starts
    public void OnPreprocessBuild(BuildReport report)
    {
        Debug.Log("[VRBuildAutomation] Starting pre-build process...");
        
        // 1. Validate project settings
        if (!ValidateProjectSettings())
        {
            throw new BuildFailedException("Project validation failed. Please fix the issues and try again.");
        }
        
        // 2. Run optimizations
        RunPreBuildOptimizations();
        
        // 3. Ensure build directory exists
        EnsureBuildDirectory();
        
        Debug.Log("[VRBuildAutomation] Pre-build process completed successfully!");
    }
    
    // Called after the build completes
    public void OnPostprocessBuild(BuildReport report)
    {
        Debug.Log($"[VRBuildAutomation] Build completed in {report.summary.totalTime.TotalSeconds:F2} seconds");
        Debug.Log($"[VRBuildAutomation] Output path: {report.summary.outputPath}");
        Debug.Log($"[VRBuildAutomation] Build size: {report.summary.totalSize / (1024f * 1024f):F2} MB");
        
        if (report.summary.result == BuildResult.Succeeded)
        {
            bool openFolder = EditorUtility.DisplayDialog(
                "Build Succeeded",
                $"Build completed successfully!\n\n" +
                $"Platform: {report.summary.platform}\n" +
                $"Size: {report.summary.totalSize / (1024f * 1024f):F2}MB\n" +
                $"Time: {report.summary.totalTime.TotalSeconds:F2} seconds\n\n" +
                $"Open build folder?",
                "Yes", "No");
                
            if (openFolder)
            {
                string folderPath = Path.GetDirectoryName(Path.GetFullPath(report.summary.outputPath));
                System.Diagnostics.Process.Start(folderPath);
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
    
    [MenuItem("VRChat/Build/All Platforms")]
    public static void BuildAllPlatforms()
    {
        if (EditorUtility.DisplayDialog("Build All Platforms", 
            "This will build for both Windows and Android (Quest). Continue?", "Yes", "Cancel"))
        {
            BuildForPlatform(BuildTarget.StandaloneWindows64);
            BuildForPlatform(BuildTarget.Android);
        }
    }
    
    [MenuItem("VRChat/Build/Windows")]
    public static void BuildWindows()
    {
        if (EditorUtility.DisplayDialog("Build for Windows", 
            "Build for Windows (PC)?", "Build", "Cancel"))
        {
            BuildForPlatform(BuildTarget.StandaloneWindows64);
        }
    }
    
    [MenuItem("VRChat/Build/Android (Quest)")]
    public static void BuildAndroid()
    {
        if (EditorUtility.DisplayDialog("Build for Android (Quest)", 
            "Build for Android (Quest)? Make sure you have the Android module installed.", "Build", "Cancel"))
        {
            BuildForPlatform(BuildTarget.Android);
        }
    }
    
    [MenuItem("VRChat/Validate Project")]
    public static void ValidateProject()
    {
        var validator = new VRProjectValidator();
        bool isValid = validator.Validate();
        
        if (isValid)
        {
            EditorUtility.DisplayDialog("Validation Complete", 
                "All project validations passed successfully!", "OK");
        }
        else
        {
            EditorUtility.DisplayDialog("Validation Failed", 
                "Some validations failed. Please check the console for details.", "OK");
        }
    }
    
    [MenuItem("VRChat/Optimize Project")]
    public static void OptimizeProject()
    {
        if (EditorUtility.DisplayDialog("Optimize Project", 
            "This will run all optimization passes. This may take some time. Continue?", 
            "Optimize", "Cancel"))
        {
            // Run texture optimizations
            var textureOptimizer = new TextureOptimizer();
            textureOptimizer.ProcessTextures();
            
            // Run mesh optimizations
            MeshOptimizer.OptimizeAllMeshes();
            
            // Run other optimizations
            RunAllOptimizations();
            
            EditorUtility.DisplayDialog("Optimization Complete", 
                "Project optimization completed successfully!", "OK");
        }
    }
    
    private static void BuildForPlatform(BuildTarget target)
    {
        // Set build options
        BuildOptions options = BuildOptions.None;
        if (EditorUserBuildSettings.development)
        {
            options |= BuildOptions.Development | BuildOptions.AllowDebugging;
        }
        
        // Configure platform-specific settings
        ConfigurePlatform(target);
        
        // Get output path
        string outputPath = GetBuildPath(target);
        string directory = Path.GetDirectoryName(outputPath);
        
        if (!Directory.Exists(directory))
        {
            Directory.CreateDirectory(directory);
        }
        
        // Build the player
        BuildPipeline.BuildPlayer(GetEnabledScenes(), outputPath, target, options);
    }
    
    private static void ConfigurePlatform(BuildTarget target)
    {
        if (target == BuildTarget.Android)
        {
            // Android (Quest) specific settings
            PlayerSettings.Android.minSdkVersion = AndroidSdkVersions.AndroidApiLevel24;
            PlayerSettings.Android.targetSdkVersion = AndroidSdkVersions.AndroidApiLevel31;
            PlayerSettings.Android.targetArchitectures = AndroidArchitecture.ARM64;
            PlayerSettings.Android.bundleVersionCode++;
            
            // Texture compression
            EditorUserBuildSettings.androidBuildSubtarget = MobileTextureSubtarget.ASTC;
            
            // Performance optimizations
            PlayerSettings.MTRendering = true;
            PlayerSettings.graphicsJobs = true;
            PlayerSettings.gpuSkinning = false;
            
            // Oculus specific settings
            PlayerSettings.VR.oculus.goLowOverheadMode = true;
            PlayerSettings.VR.oculus.sharedDepthBuffer = false;
            PlayerSettings.VR.oculus.dashSupport = true;
        }
        else if (target == BuildTarget.StandaloneWindows || target == BuildTarget.StandaloneWindows64)
        {
            // Windows specific settings
            PlayerSettings.fullScreenMode = FullScreenMode.Windowed;
            PlayerSettings.defaultIsFullScreen = false;
            PlayerSettings.defaultIsNativeResolution = true;
            PlayerSettings.visibleInBackground = false;
            PlayerSettings.allowFullscreenSwitch = true;
            
            // Performance optimizations
            PlayerSettings.MTRendering = true;
            PlayerSettings.graphicsJobs = true;
        }
    }
    
    private static string GetBuildPath(BuildTarget target)
    {
        string platform = target == BuildTarget.Android ? "Android" : "Windows";
        string extension = target == BuildTarget.Android ? ".apk" : ".exe";
        string productName = PlayerSettings.productName.Replace(" ", "");
        
        return Path.Combine(BuildConfig.BuildPath, platform, $"{productName}{extension}");
    }
    
    private static string[] GetEnabledScenes()
    {
        // First check if we have required scenes
        var enabledScenes = new List<string>();
        
        foreach (string scene in BuildConfig.RequiredScenes)
        {
            if (File.Exists(scene))
            enabledScenes.Add(scene);
        }
        
        // Fall back to build settings if no required scenes found
        if (enabledScenes.Count == 0)
        {
            foreach (EditorBuildSettingsScene scene in EditorBuildSettings.scenes)
            if (scene.enabled)
                enabledScenes.Add(scene.path);
        }
        
        if (enabledScenes.Count == 0)
            Debug.LogError("No scenes found to build!");
            
        return enabledScenes.ToArray();
    }
    
    private static bool ValidateProjectSettings()
    {
        var validator = new VRProjectValidator();
        return validator.Validate();
    }
    
    private static void RunPreBuildOptimizations()
    {
        Debug.Log("[VRBuildAutomation] Running pre-build optimizations...");
        
        // 1. Optimize textures
        var textureOptimizer = new TextureOptimizer();
        textureOptimizer.ProcessTextures();
        
        // 2. Optimize meshes
        MeshOptimizer.OptimizeAllMeshes();
        
        // 3. Run other optimizations
        RunAllOptimizations();
        
        // 4. Force garbage collection
        System.GC.Collect();
        
        Debug.Log("[VRBuildAutomation] Pre-build optimizations completed!");
    }
    
    private static void RunAllOptimizations()
    {
        // 1. Disable vsync (handled by VRChat)
        QualitySettings.vSyncCount = 0;
        
        // 2. Set target frame rate
        Application.targetFrameRate = 90; // Standard for VR
        
        // 3. Optimize physics
        Physics.autoSimulation = true;
        Physics.autoSyncTransforms = false;
        Physics.reuseCollisionCallbacks = true;
        
        // 4. Optimize audio
        AudioSettings.speakerMode = AudioSpeakerMode.Stereo;
        
        // 5. Optimize rendering
        if (QualitySettings.GetQualityLevel() < QualitySettings.names.Length - 1)
        {
            QualitySettings.IncreaseLevel(true);
        }
        
        // 6. Save all assets
        AssetDatabase.SaveAssets();
    }
    
    private static void EnsureBuildDirectory()
    {
        if (!Directory.Exists(BuildConfig.BuildPath))
        {
            Directory.CreateDirectory(BuildConfig.BuildPath);
        }
    }
}

// Helper class for project validation
public class VRProjectValidator
{
    private List<string> validationErrors = new List<string>();
    
    public bool Validate()
    {
        validationErrors.Clear();
        
        // 1. Check for required components
        CheckRequiredComponents();
        
        // 2. Check project settings
        CheckProjectSettings();
        
        // 3. Check scene setup
        CheckSceneSetup();
        
        // 4. Check performance
        CheckPerformance();
        
        // Log all errors
        foreach (string error in validationErrors)
        {
            Debug.LogError($"[Validation] {error}");
        }
        
        return validationErrors.Count == 0;
    }
    
    private void CheckRequiredComponents()
    {
        // Check for VRChat SDK
        if (!System.AppDomain.CurrentDomain.GetAssemblies().Any(a => a.FullName.StartsWith("VRCSDK")))
        {
            validationErrors.Add("VRChat SDK3 - Worlds is not installed. Please install via VCC.");
        }
        
        // Check for UdonSharp
        if (!System.AppDomain.CurrentDomain.GetAssemblies().Any(a => a.FullName.StartsWith("UdonSharp")))
        {
            validationErrors.Add("UdonSharp is not installed. Please install via VCC.");
        }
    }
    
    private void CheckProjectSettings()
    {
        // Check project name
        if (string.IsNullOrEmpty(PlayerSettings.productName) || PlayerSettings.productName.Length < 3)
        {
            validationErrors.Add("Project name is too short. Please set a proper name in Player Settings.");
        }
        
        // Check version
        if (string.IsNullOrEmpty(PlayerSettings.bundleVersion))
        {
            validationErrors.Add("Bundle version is not set. Please set it in Player Settings.");
        }
        
        // Check Android settings if building for Android
        if (EditorUserBuildSettings.activeBuildTarget == BuildTarget.Android)
        {
            if (PlayerSettings.Android.minSdkVersion < AndroidSdkVersions.AndroidApiLevel24)
            {
                validationErrors.Add("Minimum API level should be at least 24 (Android 7.0) for Quest compatibility.");
            }
            
            if (PlayerSettings.Android.targetArchitectures != AndroidArchitecture.ARM64)
            {
                validationErrors.Add("Target Architecture should be ARM64 for Quest.");
            }
        }
    }
    
    private void CheckSceneSetup()
    {
        // Check for VRCWorld component
        if (Object.FindObjectOfType<VRC.SDK3.Components.VRCWorld>() == null)
        {
            validationErrors.Add("No VRCWorld component found in the scene. Please add one.");
        }
        
        // Check for spawn points
        if (Object.FindObjectsOfType<VRC.SDK3.Components.VRCSpawnPoint>().Length == 0)
        {
            validationErrors.Add("No spawn points found in the scene. Please add at least one.");
        }
        
        // Check if scene is in build settings
        string currentScene = UnityEditor.SceneManagement.EditorSceneManager.GetActiveScene().path;
        if (!EditorBuildSettings.scenes.Any(s => s.path == currentScene && s.enabled))
        {
            validationErrors.Add("Current scene is not in build settings or is disabled. Please add it to build settings.");
        }
    }
    
    private void CheckPerformance()
    {
        // Check for high-poly meshes
        var allMeshes = Resources.FindObjectsOfTypeAll<MeshFilter>();
        int highPolyMeshes = allMeshes.Count(m => m.sharedMesh != null && m.sharedMesh.vertexCount > 5000);
        if (highPolyMeshes > 5)
        {
            validationErrors.Add($"Found {highPolyMeshes} high-poly meshes (vertex count > 5000). Consider optimizing them.");
        }
        
        // Check for large textures
        var textures = Resources.FindObjectsOfTypeAll<Texture2D>();
        int largeTextures = textures.Count(t => t.width * t.height > 2048 * 2048);
        if (largeTextures > 3)
        {
            validationErrors.Add($"Found {largeTextures} large textures (>2048x2048). Consider resizing them.");
        }
        
        // Check for real-time lights
        var lights = Object.FindObjectsOfType<Light>();
        int realtimeLights = lights.Count(l => l.lightmapBakeType != LightmapBakeType.Baked);
        if (realtimeLights > 3)
        {
            validationErrors.Add($"Found {realtimeLights} real-time lights. Consider baking some lights.");
        }
    }
}
