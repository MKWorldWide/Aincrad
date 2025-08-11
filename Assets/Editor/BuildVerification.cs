using UnityEditor;
using UnityEngine;
using System.Collections.Generic;
using System.Linq;
using System.IO;

public class BuildVerification : EditorWindow
{
    private Vector2 scrollPosition;
    private Dictionary<string, bool> verificationChecks = new Dictionary<string, bool>();
    private List<string> missingComponents = new List<string>();
    private List<string> optimizationTips = new List<string>();
    
    [MenuItem("VRChat/Verify Build Readiness")]
    public static void ShowWindow()
    {
        GetWindow<BuildVerification>("Build Verification");
    }
    
    private void OnEnable()
    {
        RunVerificationChecks();
    }
    
    private void OnGUI()
    {
        GUILayout.Label("Build Readiness Check", EditorStyles.boldLabel);
        EditorGUILayout.Space();
        
        if (GUILayout.Button("Run Verification Again"))
        {
            RunVerificationChecks();
        }
        
        EditorGUILayout.Space();
        
        bool allChecksPassed = verificationChecks.Values.All(v => v);
        
        if (allChecksPassed)
        {
            EditorGUILayout.HelpBox("✓ All build verification checks passed! You're ready to build.", MessageType.Info);
        }
        else
        {
            EditorGUILayout.HelpBox("Some issues need to be addressed before building:", MessageType.Warning);
        }
        
        scrollPosition = EditorGUILayout.BeginScrollView(scrollPosition);
        
        // Display verification results
        foreach (var check in verificationChecks)
        {
            EditorGUILayout.BeginHorizontal();
            GUILayout.Label(check.Value ? "✓" : "✗", GUILayout.Width(20));
            EditorGUILayout.LabelField(check.Key);
            EditorGUILayout.EndHorizontal();
        }
        
        // Display missing components
        if (missingComponents.Count > 0)
        {
            EditorGUILayout.Space();
            GUILayout.Label("Missing Components:", EditorStyles.boldLabel);
            foreach (string component in missingComponents)
            {
                EditorGUILayout.HelpBox(component, MessageType.Error);
            }
        }
        
        // Display optimization tips
        if (optimizationTips.Count > 0)
        {
            EditorGUILayout.Space();
            GUILayout.Label("Optimization Tips:", EditorStyles.boldLabel);
            foreach (string tip in optimizationTips)
            {
                EditorGUILayout.HelpBox(tip, MessageType.Info);
            }
        }
        
        EditorGUILayout.EndScrollView();
        
        EditorGUILayout.Space();
        
        // Action buttons
        EditorGUILayout.BeginHorizontal();
        
        if (GUILayout.Button("Fix All Issues"))
        {
            FixAllIssues();
        }
        
        if (GUILayout.Button("Run Optimizations"))
        {
            RunOptimizations();
        }
        
        EditorGUI.BeginDisabledGroup(!allChecksPassed);
        if (GUILayout.Button("Build Now"))
        {
            StartBuild();
        }
        EditorGUI.EndDisabledGroup();
        
        EditorGUILayout.EndHorizontal();
    }
    
    private void RunVerificationChecks()
    {
        verificationChecks.Clear();
        missingComponents.Clear();
        optimizationTips.Clear();
        
        // Check for VRChat SDK
        bool hasVRCSDK = System.AppDomain.CurrentDomain.GetAssemblies()
            .Any(a => a.FullName.StartsWith("VRCSDK"));
        verificationChecks.Add("VRChat SDK is installed", hasVRCSDK);
        if (!hasVRCSDK) missingComponents.Add("VRChat SDK3 - Worlds is not installed. Please install via VCC.");
        
        // Check for UdonSharp
        bool hasUdonSharp = System.AppDomain.CurrentDomain.GetAssemblies()
            .Any(a => a.FullName.StartsWith("UdonSharp"));
        verificationChecks.Add("UdonSharp is installed", hasUdonSharp);
        if (!hasUdonSharp) missingComponents.Add("UdonSharp is not installed. Please install via VCC.");
        
        // Check for TextMeshPro
        bool hasTMP = Resources.FindObjectsOfTypeAll(UnityEngine.UI.Text).Any() || 
                      Resources.FindObjectsOfTypeAll<TMPro.TextMeshProUGUI>().Any();
        verificationChecks.Add("TextMeshPro is set up", hasTMP);
        if (!hasTMP) optimizationTips.Add("TextMeshPro is not being used. Consider using it for better text rendering.");
        
        // Check for VRCWorld component
        bool hasVRCWorld = FindObjectOfType<VRC.SDK3.Components.VRCWorld>() != null;
        verificationChecks.Add("VRCWorld component exists in scene", hasVRCWorld);
        if (!hasVRCWorld) missingComponents.Add("No VRCWorld component found in the scene.");
        
        // Check for spawn points
        int spawnPointCount = FindObjectsOfType<VRC.SDK3.Components.VRCSpawnPoint>().Length;
        bool hasSpawnPoints = spawnPointCount > 0;
        verificationChecks.Add($"{spawnPointCount} spawn point(s) found", hasSpawnPoints);
        if (!hasSpawnPoints) missingComponents.Add("No spawn points found in the scene.");
        
        // Check scene in build settings
        string currentScene = UnityEditor.SceneManagement.EditorSceneManager.GetActiveScene().path;
        bool sceneInBuild = EditorBuildSettings.scenes.Any(s => s.path == currentScene && s.enabled);
        verificationChecks.Add("Current scene is in build settings", sceneInBuild);
        if (!sceneInBuild) optimizationTips.Add("Current scene is not in build settings. Add it for proper building.");
        
        // Check for performance issues
        CheckPerformance();
    }
    
    private void CheckPerformance()
    {
        // Check for high-poly meshes
        var allMeshes = Resources.FindObjectsOfTypeAll<MeshFilter>();
        int highPolyMeshes = allMeshes.Count(m => m.sharedMesh != null && m.sharedMesh.vertexCount > 5000);
        verificationChecks.Add($"High-poly meshes (>{highPolyMeshes} found, <5 recommended)", highPolyMeshes <= 5);
        if (highPolyMeshes > 5) optimizationTips.Add($"Found {highPolyMeshes} high-poly meshes. Consider optimizing or using LODs.");
        
        // Check for large textures
        var textures = Resources.FindObjectsOfTypeAll<Texture2D>();
        int largeTextures = textures.Count(t => t.width * t.height > 2048 * 2048);
        verificationChecks.Add($"Large textures (>{largeTextures} found, <3 recommended)", largeTextures <= 3);
        if (largeTextures > 3) optimizationTips.Add($"Found {largeTextures} large textures. Consider resizing or compressing them.");
        
        // Check for real-time lights
        var lights = FindObjectsOfType<Light>();
        int realtimeLights = lights.Count(l => l.lightmapBakeType != LightmapBakeType.Baked);
        verificationChecks.Add($"Real-time lights ({realtimeLights} found, <3 recommended)", realtimeLights <= 3);
        if (realtimeLights > 3) optimizationTips.Add($"Found {realtimeLights} real-time lights. Consider baking some lights.");
        
        // Check for missing materials
        var renderers = FindObjectsOfType<Renderer>();
        int missingMaterials = renderers.Count(r => r.sharedMaterial == null);
        verificationChecks.Add($"Missing materials ({missingMaterials} found, 0 recommended)", missingMaterials == 0);
        if (missingMaterials > 0) optimizationTips.Add($"Found {missingMaterials} renderers with missing materials.");
        
        // Check for missing colliders on large objects
        var largeObjects = FindObjectsOfType<Transform>()
            .Where(t => t.lossyScale.magnitude > 5f && t.GetComponent<Collider>() == null)
            .ToArray();
        verificationChecks.Add($"Large objects without colliders ({largeObjects.Length} found, <3 recommended)", largeObjects.Length <= 3);
        if (largeObjects.Length > 3) optimizationTips.Add($"Found {largeObjects.Length} large objects without colliders. Add colliders for proper physics interactions.");
    }
    
    private void FixAllIssues()
    {
        // Fix missing VRCWorld component
        if (verificationChecks.ContainsKey("VRCWorld component exists in scene") && 
            !verificationChecks["VRCWorld component exists in scene"])
        {
            GameObject world = new GameObject("VRCWorld");
            world.AddComponent<VRC.SDK3.Components.VRCWorld>();
            Debug.Log("Added VRCWorld component to the scene.");
        }
        
        // Fix missing spawn points
        if (verificationChecks.ContainsKey("0 spawn point(s) found") && 
            !verificationChecks["0 spawn point(s) found"])
        {
            GameObject spawnPoint = new GameObject("SpawnPoint");
            spawnPoint.AddComponent<VRC.SDK3.Components.VRCSpawnPoint>();
            spawnPoint.transform.position = Vector3.zero;
            spawnPoint.transform.rotation = Quaternion.identity;
            Debug.Log("Added a spawn point to the scene at (0, 0, 0).");
        }
        
        // Fix scene not in build settings
        if (verificationChecks.ContainsKey("Current scene is in build settings") && 
            !verificationChecks["Current scene is in build settings"])
        {
            string currentScene = UnityEditor.SceneManagement.EditorSceneManager.GetActiveScene().path;
            var scenes = new List<EditorBuildSettingsScene>(EditorBuildSettings.scenes);
            scenes.Add(new EditorBuildSettingsScene(currentScene, true));
            EditorBuildSettings.scenes = scenes.ToArray();
            Debug.Log("Added current scene to build settings.");
        }
        
        // Save all changes
        AssetDatabase.SaveAssets();
        UnityEditor.SceneManagement.EditorSceneManager.SaveOpenScenes();
        
        // Re-run verification
        RunVerificationChecks();
    }
    
    private void RunOptimizations()
    {
        // Run texture optimization
        var textureOptimizer = new TextureOptimizer();
        textureOptimizer.OptimizeAllTextures();
        
        // Run mesh optimization
        var meshOptimizer = new MeshOptimizer();
        meshOptimizer.OptimizeAllMeshes();
        
        // Run light baking if needed
        if (FindObjectsOfType<Light>().Any(l => l.lightmapBakeType == LightmapBakeType.Mixed))
        {
            Lightmapping.BakeAsync();
            Debug.Log("Started baking lights. This may take a while...");
        }
        
        // Re-run verification
        RunVerificationChecks();
    }
    
    private void StartBuild()
    {
        // Save the scene first
        UnityEditor.SceneManagement.EditorSceneManager.SaveOpenScenes();
        
        // Show build options
        BuildOptionsWindow.ShowWindow();
    }
}

public class BuildOptionsWindow : EditorWindow
{
    private static BuildTarget buildTarget = EditorUserBuildSettings.activeBuildTarget;
    private static bool developmentBuild = false;
    
    public static void ShowWindow()
    {
        var window = GetWindow<BuildOptionsWindow>("Build Options");
        window.minSize = new Vector2(300, 150);
        window.Show();
    }
    
    private void OnGUI()
    {
        GUILayout.Label("Build Options", EditorStyles.boldLabel);
        EditorGUILayout.Space();
        
        buildTarget = (BuildTarget)EditorGUILayout.EnumPopup("Build Target", buildTarget);
        developmentBuild = EditorGUILayout.Toggle("Development Build", developmentBuild);
        
        EditorGUILayout.Space();
        
        if (GUILayout.Button("Start Build"))
        {
            BuildPlayerOptions buildPlayerOptions = new BuildPlayerOptions();
            buildPlayerOptions.scenes = EditorBuildSettings.scenes
                .Where(s => s.enabled)
                .Select(s => s.path)
                .ToArray();
                
            string buildPath = Path.Combine("Builds", buildTarget.ToString());
            if (!Directory.Exists(buildPath))
                Directory.CreateDirectory(buildPath);
                
            buildPlayerOptions.locationPathName = Path.Combine(buildPath, 
                $"{PlayerSettings.productName}{(buildTarget == BuildTarget.Android ? ".apk" : ".exe")}");
                
            buildPlayerOptions.target = buildTarget;
            buildPlayerOptions.options = developmentBuild ? 
                BuildOptions.Development | BuildOptions.AllowDebugging : 
                BuildOptions.None;
                
            BuildPipeline.BuildPlayer(buildPlayerOptions);
            
            Close();
        }
    }
}
