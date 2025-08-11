using UnityEditor;
using UnityEditor.Build;
using UnityEditor.Build.Reporting;
using UnityEngine;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using Debug = UnityEngine.Debug;

public class BuildAutomation : IPreprocessBuildWithReport, IPostprocessBuildWithReport
{
    public int callbackOrder => 0;

    // Called before the build starts
    public void OnPreprocessBuild(BuildReport report)
    {
        Debug.Log("[BuildAutomation] Starting build process...");
        
        // Create necessary directories
        string buildPath = Path.Combine(Application.dataPath, "../Builds");
        if (!Directory.Exists(buildPath))
            Directory.CreateDirectory(buildPath);

        // Optimize textures before building
        OptimizeAllTextures();
        
        // Optimize meshes
        OptimizeAllMeshes();
        
        // Update asset bundles if needed
        UpdateAssetBundles();
        
        // Clean up unused assets
        CleanUpAssets();
        
        Debug.Log("[BuildAutomation] Pre-build optimizations complete!");
    }

    // Called after the build is complete
    public void OnPostprocessBuild(BuildReport report)
    {
        Debug.Log($"[BuildAutomation] Build completed in {report.summary.totalTime.TotalSeconds:F2} seconds");
        Debug.Log($"[BuildAutomation] Output path: {report.summary.outputPath}");
        Debug.Log($"[BuildAutomation] Build size: {report.summary.totalSize / (1024f * 1024f):F2} MB");
        
        // Additional post-build steps can be added here
        if (report.summary.result == BuildResult.Succeeded)
        {
            // Notify build success
            EditorUtility.DisplayDialog("Build Complete", 
                $"Build completed successfully!\n\n" +
                $"Platform: {report.summary.platform}\n" +
                $"Size: {report.summary.totalSize / (1024f * 1024f):F2} MB\n" +
                $"Time: {report.summary.totalTime.TotalSeconds:F2} seconds", "OK");
        }
    }
    
    private void OptimizeAllTextures()
    {
        Debug.Log("[BuildAutomation] Optimizing textures...");
        
        // Get all texture paths
        string[] texturePaths = Directory.GetFiles(Application.dataPath, "*.png", SearchOption.AllDirectories)
            .Concat(Directory.GetFiles(Application.dataPath, "*.jpg", SearchOption.AllDirectories))
            .Concat(Directory.GetFiles(Application.dataPath, "*.jpeg", SearchOption.AllDirectories))
            .Concat(Directory.GetFiles(Application.dataPath, "*.tga", SearchOption.AllDirectories))
            .Where(p => !p.Contains("/Editor/") && !p.Contains("/Gizmos/") && !p.Contains("/StreamingAssets/"))
            .ToArray();
        
        foreach (string texturePath in texturePaths)
        {
            string relativePath = "Assets" + texturePath.Replace(Application.dataPath, "").Replace("\\", "/");
            TextureImporter importer = AssetImporter.GetAtPath(relativePath) as TextureImporter;
            
            if (importer != null)
            {
                bool modified = false;
                
                // Skip UI textures from compression
                if (relativePath.Contains("/UI/"))
                {
                    if (importer.textureCompression != TextureImporterCompression.Uncompressed)
                    {
                        importer.textureCompression = TextureImporterCompression.Uncompressed;
                        modified = true;
                    }
                    continue;
                }
                
                // Set Android settings
                var androidSettings = importer.GetPlatformTextureSettings("Android");
                if (!androidSettings.overridden || 
                    androidSettings.maxTextureSize > 2048 || 
                    androidSettings.format != TextureImporterFormat.ASTC_6x6)
                {
                    androidSettings.overridden = true;
                    androidSettings.maxTextureSize = Mathf.Min(importer.maxTextureSize, 2048);
                    androidSettings.format = TextureImporterFormat.ASTC_6x6;
                    importer.SetPlatformTextureSettings(androidSettings);
                    modified = true;
                }
                
                // Set PC settings
                var standaloneSettings = importer.GetPlatformTextureSettings("Standalone");
                if (!standaloneSettings.overridden || 
                    standaloneSettings.maxTextureSize > 4096 || 
                    standaloneSettings.format != TextureImporterFormat.BC7)
                {
                    standaloneSettings.overridden = true;
                    standaloneSettings.maxTextureSize = Mathf.Min(importer.maxTextureSize, 4096);
                    standaloneSettings.format = TextureImporterFormat.BC7;
                    importer.SetPlatformTextureSettings(standaloneSettings);
                    modified = true;
                }
                
                // General settings
                if (importer.isReadable || 
                    !importer.mipmapEnabled || 
                    importer.textureCompression != TextureImporterCompression.Compressed)
                {
                    importer.isReadable = false;
                    importer.mipmapEnabled = true;
                    importer.textureCompression = TextureImporterCompression.Compressed;
                    modified = true;
                }
                
                if (modified)
                {
                    importer.SaveAndReimport();
                    Debug.Log($"[BuildAutomation] Optimized texture: {relativePath}");
                }
            }
        }
        
        AssetDatabase.SaveAssets();
        AssetDatabase.Refresh();
    }
    
    private void OptimizeAllMeshes()
    {
        Debug.Log("[BuildAutomation] Optimizing meshes...");
        
        // Get all mesh paths
        string[] meshPaths = Directory.GetFiles(Application.dataPath, "*.fbx", SearchOption.AllDirectories)
            .Concat(Directory.GetFiles(Application.dataPath, "*.obj", SearchOption.AllDirectories))
            .Where(p => !p.Contains("/Editor/") && !p.Contains("/Gizmos/") && !p.Contains("/StreamingAssets/"))
            .ToArray();
        
        foreach (string meshPath in meshPaths)
        {
            string relativePath = "Assets" + meshPath.Replace(Application.dataPath, "").Replace("\\", "/");
            ModelImporter importer = AssetImporter.GetAtPath(relativePath) as ModelImporter;
            
            if (importer != null)
            {
                bool modified = false;
                
                // Enable mesh compression
                if (importer.meshCompression != ModelImporterMeshCompression.Medium)
                {
                    importer.meshCompression = ModelImporterMeshCompression.Medium;
                    modified = true;
                }
                
                // Optimize mesh
                if (!importer.optimizeMesh)
                {
                    importer.optimizeMesh = true;
                    modified = true;
                }
                
                // Generate colliders for large static objects
                if (relativePath.Contains("/Environment/"))
                {
                    if (importer.addCollider != (importer.importBlendShapes || importer.importVisibility || importer.importCameras || importer.importLights))
                    {
                        importer.addCollider = !(importer.importBlendShapes || importer.importVisibility || importer.importCameras || importer.importLights);
                        modified = true;
                    }
                }
                
                if (modified)
                {
                    importer.SaveAndReimport();
                    Debug.Log($"[BuildAutomation] Optimized mesh: {relativePath}");
                }
            }
        }
        
        AssetDatabase.SaveAssets();
        AssetDatabase.Refresh();
    }
    
    private void UpdateAssetBundles()
    {
        Debug.Log("[BuildAutomation] Updating asset bundles...");
        
        // This would be where you'd update any asset bundles
        // For now, we'll just log that we would do this
        
        // Example:
        // BuildPipeline.BuildAssetBundles("Assets/AssetBundles", 
        //     BuildAssetBundleOptions.ChunkBasedCompression | 
        //     BuildAssetBundleOptions.ForceRebuildAssetBundle, 
        //     EditorUserBuildSettings.activeBuildTarget);
        
        Debug.Log("[BuildAutomation] Asset bundles updated!");
    }
    
    private void CleanUpAssets()
    {
        Debug.Log("[BuildAutomation] Cleaning up assets...");
        
        // Clean up unused assets
        EditorUtility.UnloadUnusedAssetsImmediate();
        
        // Force garbage collection
        System.GC.Collect();
        
        Debug.Log("[BuildAutomation] Asset cleanup complete!");
    }
    
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
        Debug.Log($"[BuildAutomation] Starting {platformName} build...");
        
        // Set platform-specific settings
        if (target == BuildTarget.Android)
        {
            PlayerSettings.Android.minSdkVersion = AndroidSdkVersions.AndroidApiLevel24;
            PlayerSettings.Android.targetSdkVersion = AndroidSdkVersions.AndroidApiLevel31;
            PlayerSettings.Android.targetArchitectures = AndroidArchitecture.ARM64;
            PlayerSettings.Android.bundleVersionCode++;
            EditorUserBuildSettings.androidBuildSubtarget = MobileTextureSubtarget.ASTC;
        }
        
        // Define build options
        BuildOptions options = BuildOptions.None;
        
        // For development builds
        if (EditorUserBuildSettings.development)
        {
            options |= BuildOptions.Development | BuildOptions.AllowDebugging;
        }
        
        // Set build path
        string buildPath = Path.Combine(Application.dataPath, "../Builds", platformName);
        string executableName = PlayerSettings.productName.Replace(" ", "");
        
        if (!Directory.Exists(buildPath))
            Directory.CreateDirectory(buildPath);
        
        // Get scenes to build
        List<string> scenes = new List<string>();
        foreach (EditorBuildSettingsScene scene in EditorBuildSettings.scenes)
        {
            if (scene.enabled)
                scenes.Add(scene.path);
        }
        
        if (scenes.Count == 0)
        {
            Debug.LogError("[BuildAutomation] No scenes found in build settings!");
            return;
        }
        
        // Set output path
        string outputPath;
        if (target == BuildTarget.Android)
        {
            outputPath = Path.Combine(buildPath, $"{executableName}.apk");
        }
        else
        {
            outputPath = Path.Combine(buildPath, $"{executableName}.exe");
        }
        
        // Start the build
        BuildPipeline.BuildPlayer(scenes.ToArray(), outputPath, target, options);
        
        Debug.Log($"[BuildAutomation] {platformName} build complete: {outputPath}");
    }
}
