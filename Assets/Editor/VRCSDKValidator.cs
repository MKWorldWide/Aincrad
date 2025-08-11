using UnityEditor;
using UnityEngine;
using VRC.SDKBase.Editor.Build;
using VRC.SDKBase.Editor;
using System.Collections.Generic;
using System.Linq;

public class VRCSDKValidator : EditorWindow
{
    private Vector2 scrollPosition;
    private Dictionary<string, bool> checks = new Dictionary<string, bool>();
    private bool allChecksPassed = false;
    
    [MenuItem("VRChat/Validate SDK Setup")]
    public static void ShowWindow()
    {
        GetWindow<VRCSDKValidator>("VRChat SDK Validator");
    }
    
    private void OnEnable()
    {
        RunChecks();
    }
    
    private void OnGUI()
    {
        GUILayout.Label("VRChat SDK Validator", EditorStyles.boldLabel);
        EditorGUILayout.Space();
        
        if (GUILayout.Button("Run Checks Again"))
        {
            RunChecks();
        }
        
        EditorGUILayout.Space();
        
        if (allChecksPassed)
        {
            EditorGUILayout.HelpBox("All VRChat SDK checks passed! You're ready to build.", MessageType.Info);
        }
        else
        {
            EditorGUILayout.HelpBox("Some checks failed. Please fix the issues below before building.", MessageType.Warning);
        }
        
        EditorGUILayout.Space();
        
        scrollPosition = EditorGUILayout.BeginScrollView(scrollPosition);
        
        foreach (var check in checks)
        {
            EditorGUILayout.BeginHorizontal();
            
            // Status icon
            GUIStyle style = new GUIStyle(EditorStyles.label);
            style.padding = new RectOffset(20, 0, 0, 0);
            
            if (check.Value)
            {
                GUILayout.Label("✓", GUILayout.Width(20));
                EditorGUILayout.LabelField(check.Key, style);
            }
            else
            {
                GUILayout.Label("✗", GUILayout.Width(20));
                EditorGUILayout.LabelField(check.Key, style);
            }
            
            EditorGUILayout.EndHorizontal();
        }
        
        EditorGUILayout.EndScrollView();
        
        if (!allChecksPassed)
        {
            EditorGUILayout.Space();
            if (GUILayout.Button("Fix All Issues"))
            {
                FixAllIssues();
            }
        }
    }
    
    private void RunChecks()
    {
        checks.Clear();
        allChecksPassed = true;
        
        // Check if VRCSDK is imported
        bool hasVRCSDK = System.AppDomain.CurrentDomain.GetAssemblies()
            .Any(a => a.FullName.StartsWith("VRCSDK"));
            
        checks.Add("VRCSDK is imported", hasVRCSDK);
        
        // Check if UdonSharp is imported
        bool hasUdonSharp = System.AppDomain.CurrentDomain.GetAssemblies()
            .Any(a => a.FullName.StartsWith("UdonSharp"));
            
        checks.Add("UdonSharp is imported", hasUdonSharp);
        
        // Check if VRC World component exists in the scene
        bool hasVRCSDKWorld = FindObjectOfType<VRC.SDK3.Components.VRCWorld>() != null;
        checks.Add("VRCWorld component in scene", hasVRCSDKWorld);
        
        // Check if there's at least one spawn point
        bool hasSpawnPoint = FindObjectsOfType<VRC.SDK3.Components.VRCSpawnPoint>().Length > 0;
        checks.Add("At least one spawn point exists", hasSpawnPoint);
        
        // Check if the scene is in the build settings
        bool sceneInBuild = false;
        string currentScene = UnityEditor.SceneManagement.EditorSceneManager.GetActiveScene().path;
        
        foreach (var scene in EditorBuildSettings.scenes)
        {
            if (scene.path == currentScene && scene.enabled)
            {
                sceneInBuild = true;
                break;
            }
        }
        
        checks.Add("Current scene is in build settings and enabled", sceneInBuild);
        
        // Check if the project has a valid VRChat ID
        bool hasValidId = !string.IsNullOrEmpty(VRC.SDKBase.Editor.VRCProjectConfig.GetProjectConfig().projectId);
        checks.Add("Project has a valid VRChat ID", hasValidId);
        
        // Check if the project has a valid content rating
        bool hasContentRating = VRC.SDKBase.Editor.VRCProjectConfig.GetProjectConfig().contentRating != VRC.SDKBase.VRC_SceneDescriptor.Rating.Unknown;
        checks.Add("Project has a valid content rating", hasContentRating);
        
        // Check if the project has a valid name
        bool hasValidName = !string.IsNullOrEmpty(PlayerSettings.productName) && 
                           PlayerSettings.productName.Length >= 3 && 
                           PlayerSettings.productName.Length <= 30;
        checks.Add("Project has a valid name (3-30 chars)", hasValidName);
        
        // Check if the project has a valid version
        bool hasValidVersion = !string.IsNullOrEmpty(PlayerSettings.bundleVersion);
        checks.Add("Project has a valid version number", hasValidVersion);
        
        // Check if all required layers are set up
        bool hasRequiredLayers = true;
        string[] requiredLayers = new string[] 
        { 
            "Player", "PlayerLocal", "PlayerNetwork", "Environment", "Interactive", 
            "UI", "Pickup", "PickupNoEnvironment", "MirrorReflection", "UI_Internal" 
        };
        
        foreach (string layer in requiredLayers)
        {
            if (LayerMask.NameToLayer(layer) == -1)
            {
                hasRequiredLayers = false;
                break;
            }
        }
        
        checks.Add("All required layers are set up", hasRequiredLayers);
        
        // Check if all required tags are set up
        bool hasRequiredTags = true;
        string[] requiredTags = new string[] 
        { 
            "Player", "LocalPlayer", "PlayerLocal", "PlayerNetwork", "Interactive", 
            "World", "Pickup", "PickupNoEnvironment", "EditorOnly", "MainCamera", 
            "MirrorReflection", "UI", "MirrorReflection" 
        };
        
        foreach (string tag in requiredTags)
        {
            try
            {
                if (!UnityEditorInternal.InternalEditorUtility.tags.Contains(tag))
                {
                    hasRequiredTags = false;
                    break;
                }
            }
            catch
            {
                hasRequiredTags = false;
                break;
            }
        }
        
        checks.Add("All required tags are set up", hasRequiredTags);
        
        // Update overall status
        allChecksPassed = !checks.Values.Contains(false);
    }
    
    private void FixAllIssues()
    {
        // Fix missing VRCWorld component
        if (!checks["VRCWorld component in scene"])
        {
            GameObject world = new GameObject("VRCWorld");
            world.AddComponent<VRC.SDK3.Components.VRCWorld>();
            Debug.Log("Added VRCWorld component to the scene.");
        }
        
        // Fix missing spawn point
        if (!checks["At least one spawn point exists"])
        {
            GameObject spawnPoint = new GameObject("SpawnPoint");
            spawnPoint.AddComponent<VRC.SDK3.Components.VRCSpawnPoint>();
            spawnPoint.transform.position = Vector3.zero;
            spawnPoint.transform.rotation = Quaternion.identity;
            Debug.Log("Added a spawn point to the scene at (0, 0, 0).");
        }
        
        // Fix scene not in build settings
        if (!checks["Current scene is in build settings and enabled"])
        {
            string currentScene = UnityEditor.SceneManagement.EditorSceneManager.GetActiveScene().path;
            var scenes = new List<EditorBuildSettingsScene>(EditorBuildSettings.scenes);
            scenes.Add(new EditorBuildSettingsScene(currentScene, true));
            EditorBuildSettings.scenes = scenes.ToArray();
            Debug.Log("Added current scene to build settings.");
        }
        
        // Fix missing project ID
        if (!checks["Project has a valid VRChat ID"])
        {
            var config = VRC.SDKBase.Editor.VRCProjectConfig.GetProjectConfig();
            config.projectId = System.Guid.NewGuid().ToString();
            EditorUtility.SetDirty(config);
            Debug.Log("Generated a new VRChat project ID.");
        }
        
        // Fix missing content rating
        if (!checks["Project has a valid content rating"])
        {
            var config = VRC.SDKBase.Editor.VRCProjectConfig.GetProjectConfig();
            config.contentRating = VRC.SDKBase.VRC_SceneDescriptor.Rating.Mature;
            EditorUtility.SetDirty(config);
            Debug.Log("Set content rating to Mature. Please adjust as needed.");
        }
        
        // Fix project name
        if (!checks["Project has a valid name (3-30 chars)"])
        {
            PlayerSettings.productName = "Aincrad";
            Debug.Log("Set project name to 'Aincrad'.");
        }
        
        // Fix version
        if (!checks["Project has a valid version number"])
        {
            PlayerSettings.bundleVersion = "1.0.0";
            Debug.Log("Set project version to '1.0.0'.");
        }
        
        // Run checks again to update status
        RunChecks();
        
        if (allChecksPassed)
        {
            Debug.Log("All issues have been fixed!");
        }
        else
        {
            Debug.LogWarning("Some issues could not be fixed automatically. Please check the console for details.");
        }
        
        // Save all changes
        AssetDatabase.SaveAssets();
        EditorSceneManager.SaveOpenScenes();
    }
}
