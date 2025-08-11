using UnityEngine;
using UnityEditor;
using UnityEditor.Build;
using UnityEditor.Build.Reporting;
using System.IO;
using System.Collections.Generic;
using System.Diagnostics;
using Debug = UnityEngine.Debug;

public class DeploymentScript : EditorWindow
{
    private string buildPath = "Builds";
    private string worldName = "GlasswaterHub";
    private string version = "1.0.0";
    private bool developmentBuild = false;
    private bool autoUpload = true;
    private bool optimizeForQuest = true;
    private bool showAdvanced = false;

    [MenuItem("VRChat/Deployment Tool")]
    public static void ShowWindow()
    {
        GetWindow<DeploymentScript>("VRChat Deployment");
    }

    private void OnGUI()
    {
        GUILayout.Label("VRChat World Deployment", EditorStyles.boldLabel);
        EditorGUILayout.Space();

        worldName = EditorGUILayout.TextField("World Name", worldName);
        version = EditorGUILayout.TextField("Version", version);
        buildPath = EditorGUILayout.TextField("Build Path", buildPath);

        EditorGUILayout.Space();
        developmentBuild = EditorGUILayout.Toggle("Development Build", developmentBuild);
        autoUpload = EditorGUILayout.Toggle("Auto Upload to VRChat", autoUpload);
        optimizeForQuest = EditorGUILayout.Toggle("Optimize for Quest", optimizeForQuest);

        showAdvanced = EditorGUILayout.Foldout(showAdvanced, "Advanced Settings");
        if (showAdvanced)
        {
            EditorGUI.indentLevel++;
            // Add any advanced settings here
            EditorGUI.indentLevel--;
        }

        EditorGUILayout.Space();

        if (GUILayout.Button("Build & Deploy"))
        {
            BuildAndDeploy();
        }

        EditorGUILayout.Space();

        if (GUILayout.Button("Open Build Folder"))
        {
            if (Directory.Exists(buildPath))
            Process.Start(Path.GetFullPath(buildPath));
            else
                Debug.LogError("Build directory does not exist: " + buildPath);
        }
    }

    private void BuildAndDeploy()
    {
        try
        {
            // Ensure build directory exists
            if (!Directory.Exists(buildPath))
                Directory.CreateDirectory(buildPath);

            // Build for Windows
            BuildWindows();

            // Build for Android (Quest) if enabled
            if (optimizeForQuest)
            {
                BuildAndroid();
            }

            // Auto-upload if enabled
            if (autoUpload)
            {
                OpenVRCSDKPanel();
            }

            Debug.Log("Build and deployment process completed successfully!");
        }
        catch (System.Exception e)
        {
            Debug.LogError("Build and deployment failed: " + e.Message);
            EditorUtility.DisplayDialog("Build Failed", "Build and deployment failed: " + e.Message, "OK");
        }
    }

    private void BuildWindows()
    {
        string exeName = $"{worldName}_v{version}" + (developmentBuild ? "_Dev" : "") + ".exe";
        string fullPath = Path.Combine(buildPath, "Windows", exeName);
        string directory = Path.GetDirectoryName(fullPath);

        if (!Directory.Exists(directory))
            Directory.CreateDirectory(directory);

        BuildPlayerOptions buildOptions = new BuildPlayerOptions
        {
            scenes = GetEnabledScenes(),
            locationPathName = fullPath,
            target = BuildTarget.StandaloneWindows64,
            options = developmentBuild ? 
                BuildOptions.Development | BuildOptions.AllowDebugging : 
                BuildOptions.None
        };

        // Apply optimizations
        OptimizeForWindows();

        // Build
        BuildReport report = BuildPipeline.BuildPlayer(buildOptions);
        BuildSummary summary = report.summary;

        if (summary.result == BuildResult.Succeeded)
        {
            Debug.Log($"Windows build succeeded: {summary.totalSize / (1024f * 1024f):F2} MB");
            Process.Start("explorer.exe", $"/select,\"{Path.GetFullPath(fullPath)}\"");
        }
        else
        {
            throw new System.Exception("Windows build failed");
        }
    }

    private void BuildAndroid()
    {
        // Switch to Android platform
        if (EditorUserBuildSettings.activeBuildTarget != BuildTarget.Android)
        {
            EditorUserBuildSettings.SwitchActiveBuildTarget(BuildTargetGroup.Android, BuildTarget.Android);
            // Wait for the platform switch to complete
            System.Threading.Thread.Sleep(1000);
        }

        string apkName = $"{worldName}_Quest_v{version}.apk";
        string fullPath = Path.Combine(buildPath, "Android", apkName);
        string directory = Path.GetDirectoryName(fullPath);

        if (!Directory.Exists(directory))
            Directory.CreateDirectory(directory);

        // Configure Android settings
        PlayerSettings.Android.minSdkVersion = AndroidSdkVersions.AndroidApiLevel24;
        PlayerSettings.Android.targetSdkVersion = AndroidSdkVersions.AndroidApiLevel31;
        PlayerSettings.Android.targetArchitectures = AndroidArchitecture.ARM64;
        PlayerSettings.Android.bundleVersionCode++;

        // Set up build
        BuildPlayerOptions buildOptions = new BuildPlayerOptions
        {
            scenes = GetEnabledScenes(),
            locationPathName = fullPath,
            target = BuildTarget.Android,
            options = developmentBuild ? 
                BuildOptions.Development | BuildOptions.AllowDebugging : 
                BuildOptions.None
        };

        // Apply Quest optimizations
        OptimizeForQuest();

        // Build
        BuildReport report = BuildPipeline.BuildPlayer(buildOptions);
        BuildSummary summary = report.summary;

        if (summary.result == BuildResult.Succeeded)
        {
            Debug.Log($"Android (Quest) build succeeded: {summary.totalSize / (1024f * 1024f):F2} MB");
            EditorUtility.RevealInFinder(fullPath);
        }
        else
        {
            throw new System.Exception("Android (Quest) build failed");
        }
    }

    private void OptimizeForWindows()
    {
        // Graphics settings
        PlayerSettings.SetGraphicsAPIs(BuildTarget.StandaloneWindows64, 
            new[] { UnityEngine.Rendering.GraphicsDeviceType.Direct3D11 });
        
        // Quality settings
        QualitySettings.vSyncCount = 0;
        QualitySettings.antiAliasing = 2; // 2x MSAA
        QualitySettings.pixelLightCount = 2;
        QualitySettings.shadowDistance = 50;
        QualitySettings.shadowResolution = ShadowResolution.Medium;
        QualitySettings.shadowCascades = 2;
        
        // Player settings
        PlayerSettings.colorSpace = ColorSpace.Linear;
        PlayerSettings.gpuSkinning = true;
        PlayerSettings.graphicsJobs = true;
        
        // Build settings
        EditorUserBuildSettings.enableHeadlessMode = false;
        EditorUserBuildSettings.standaloneBuildSubtarget = StandaloneBuildSubtarget.Player;
    }

    private void OptimizeForQuest()
    {
        // Graphics settings
        PlayerSettings.SetMobileMTRendering(BuildTargetGroup.Android, true);
        PlayerSettings.MTRendering = true;
        
        // Quality settings
        QualitySettings.vSyncCount = 0;
        QualitySettings.antiAliasing = 0; // No AA on Quest
        QualitySettings.pixelLightCount = 1;
        QualitySettings.shadowDistance = 30;
        QualitySettings.shadowResolution = ShadowResolution.Low;
        QualitySettings.shadowCascades = 1;
        
        // Player settings
        PlayerSettings.colorSpace = ColorSpace.Gamma;
        PlayerSettings.gpuSkinning = false;
        PlayerSettings.graphicsJobs = false;
        
        // Android settings
        PlayerSettings.Android.minSdkVersion = AndroidSdkVersions.AndroidApiLevel24;
        PlayerSettings.Android.targetSdkVersion = AndroidSdkVersions.AndroidApiLevel31;
        PlayerSettings.Android.targetArchitectures = AndroidArchitecture.ARM64;
        PlayerSettings.Android.androidTVCompatibility = false;
        PlayerSettings.Android.androidIsGame = true;
        
        // Texture compression
        EditorUserBuildSettings.androidBuildSubtarget = MobileTextureSubtarget.ASTC;
    }

    private string[] GetEnabledScenes()
    {
        List<string> enabledScenes = new List<string>();
        
        foreach (EditorBuildSettingsScene scene in EditorBuildSettings.scenes)
        {
            if (scene.enabled)
                enabledScenes.Add(scene.path);
        }
        
        if (enabledScenes.Count == 0)
        {
            // If no scenes are in build settings, use the active scene
            if (UnityEngine.SceneManagement.SceneManager.GetActiveScene().isLoaded)
            {
                enabledScenes.Add(UnityEngine.SceneManagement.SceneManager.GetActiveScene().path);
            }
            else
            {
                throw new System.Exception("No scenes found in build settings and no active scene!");
            }
        }
        
        return enabledScenes.ToArray();
    }

    private void OpenVRCSDKPanel()
    {
        System.Type sdkWindowType = System.Type.GetType("VRC.SDKBase.Editor.VRC_SdkBuilder, VRCSdkEditor");
        if (sdkWindowType != null)
        {
            EditorWindow.GetWindow(sdkWindowType);
            Debug.Log("Please use the VRChat SDK control panel to upload your world.");
        }
        else
        {
            Debug.LogWarning("VRChat SDK not found. Please install the VRChat Creator Companion and import the SDK.");
        }
    }
}
