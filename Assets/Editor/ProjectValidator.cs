using UnityEditor;
using UnityEditor.Build;
using UnityEditor.Build.Reporting;
using UnityEngine;
using System.Collections.Generic;
using System.Linq;
using System.IO;

[InitializeOnLoad]
public class ProjectValidator : IPreprocessBuildWithReport
{
    public int callbackOrder => 0;
    
    private static readonly string[] RequiredTags = 
    {
        "Player", "LocalPlayer", "PlayerLocal", "PlayerNetwork", "Interactive",
        "World", "Pickup", "PickupNoEnvironment", "EditorOnly", "MainCamera",
        "MirrorReflection", "UI"
    };
    
    private static readonly string[] RequiredLayers =
    {
        "Player", "PlayerLocal", "PlayerNetwork", "Environment", "Interactive",
        "UI", "Pickup", "PickupNoEnvironment", "MirrorReflection", "UI_Internal"
    };
    
    static ProjectValidator()
    {
        // Automatically validate project settings when scripts are reloaded
        EditorApplication.delayCall += ValidateProjectSettings;
    }
    
    [MenuItem("VRChat/Validate Project Settings")]
    public static void ValidateProjectSettings()
    {
        bool allValid = true;
        List<string> messages = new List<string>();
        
        // Check tags
        var missingTags = RequiredTags.Where(tag => !TagExists(tag)).ToList();
        if (missingTags.Count > 0)
        {
            allValid = false;
            messages.Add($"Missing required tags: {string.Join(", ", missingTags)}");
        }
        
        // Check layers
        var missingLayers = RequiredLayers.Where(layer => LayerExists(layer) == -1).ToList();
        if (missingLayers.Count > 0)
        {
            allValid = false;
            messages.Add($"Missing required layers: {string.Join(", ", missingLayers)}");
        }
        
        // Check VRChat SDK components
        bool hasVRCSDK = System.AppDomain.CurrentDomain.GetAssemblies()
            .Any(a => a.FullName.StartsWith("VRCSDK"));
            
        if (!hasVRCSDK)
        {
            allValid = false;
            messages.Add("VRCSDK is not imported. Please import the VRChat SDK.");
        }
        
        // Check UdonSharp
        bool hasUdonSharp = System.AppDomain.CurrentDomain.GetAssemblies()
            .Any(a => a.FullName.StartsWith("UdonSharp"));
            
        if (!hasUdonSharp)
        {
            messages.Add("UdonSharp is not imported. Some scripts may not work correctly.");
        }
        
        // Check for VRCWorld component in the scene
        var vrcWorld = Object.FindObjectOfType<VRC.SDK3.Components.VRCWorld>();
        if (vrcWorld == null)
        {
            allValid = false;
            messages.Add("No VRCWorld component found in the scene. Please add one.");
        }
        
        // Check for spawn points
        var spawnPoints = Object.FindObjectsOfType<VRC.SDK3.Components.VRCSpawnPoint>();
        if (spawnPoints.Length == 0)
        {
            allValid = false;
            messages.Add("No spawn points found in the scene. Please add at least one.");
        }
        
        // Check if scene is in build settings
        string currentScene = UnityEditor.SceneManagement.EditorSceneManager.GetActiveScene().path;
        bool sceneInBuild = false;
        
        foreach (var scene in EditorBuildSettings.scenes)
        {
            if (scene.path == currentScene && scene.enabled)
            {
                sceneInBuild = true;
                break;
            }
        }
        
        if (!sceneInBuild)
        {
            allValid = false;
            messages.Add("Current scene is not in build settings or is disabled.");
        }
        
        // Show results
        if (allValid && messages.Count == 0)
        {
            EditorUtility.DisplayDialog("Validation Complete", 
                "All project settings are valid!", "OK");
        }
        else
        {
            string message = allValid ? 
                "Project validation completed with warnings:" : 
                "Project validation failed. Please fix the following issues:";
                
            message += "\n\n" + string.Join("\n• ", messages);
            
            if (allValid)
            {
                EditorUtility.DisplayDialog("Validation Complete", message, "OK");
            }
            else
            {
                bool fix = EditorUtility.DisplayDialog("Validation Failed", 
                    message + "\n\nWould you like to fix these issues now?", "Yes", "No");
                    
                if (fix)
                {
                    FixAllIssues(missingTags, missingLayers, vrcWorld == null, spawnPoints.Length == 0, !sceneInBuild);
                }
            }
        }
    }
    
    private static void FixAllIssues(List<string> missingTags, List<string> missingLayers, 
        bool addVRCWorld, bool addSpawnPoint, bool addSceneToBuild)
    {
        // Fix missing tags
        foreach (string tag in missingTags)
        {
            AddTag(tag);
        }
        
        // Fix missing layers
        foreach (string layer in missingLayers)
        {
            AddLayer(layer);
        }
        
        // Add VRCWorld component if missing
        if (addVRCWorld)
        {
            GameObject world = new GameObject("VRCWorld");
            world.AddComponent<VRC.SDK3.Components.VRCWorld>();
            Debug.Log("Added VRCWorld component to the scene.");
        }
        
        // Add spawn point if missing
        if (addSpawnPoint)
        {
            GameObject spawnPoint = new GameObject("SpawnPoint");
            spawnPoint.AddComponent<VRC.SDK3.Components.VRCSpawnPoint>();
            spawnPoint.transform.position = Vector3.zero;
            spawnPoint.transform.rotation = Quaternion.identity;
            Debug.Log("Added a spawn point to the scene at (0, 0, 0).");
        }
        
        // Add scene to build settings if missing
        if (addSceneToBuild)
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
        
        // Re-run validation
        EditorApplication.delayCall += () => 
        {
            EditorApplication.delayCall += () => 
            {
                ValidateProjectSettings();
            };
        };
    }
    
    private static bool TagExists(string tag)
    {
        try
        {
            return UnityEditorInternal.InternalEditorUtility.tags.Contains(tag);
        }
        catch
        {
            return false;
        }
    }
    
    private static int LayerExists(string layerName)
    {
        for (int i = 0; i < 32; i++)
        {
            if (LayerMask.LayerToName(i) == layerName)
                return i;
        }
        return -1;
    }
    
    private static void AddTag(string tag)
    {
        if (TagExists(tag)) return;
        
        var asset = AssetDatabase.LoadMainAssetAtPath("ProjectSettings/TagManager.asset");
        if (asset != null)
        {
            var so = new SerializedObject(asset);
            var tags = so.FindProperty("tags");
            
            for (int i = 0; i < tags.arraySize; i++)
            {
                if (tags.GetArrayElementAtIndex(i).stringValue == tag)
                    return;
            }
            
            tags.InsertArrayElementAtIndex(tags.arraySize);
            tags.GetArrayElementAtIndex(tags.arraySize - 1).stringValue = tag;
            so.ApplyModifiedProperties();
            so.Update();
            
            Debug.Log($"Added tag: {tag}");
        }
    }
    
    private static void AddLayer(string layerName)
    {
        if (LayerExists(layerName) != -1) return;
        
        var tagManager = new SerializedObject(AssetDatabase.LoadMainAssetAtPath("ProjectSettings/TagManager.asset"));
        var layers = tagManager.FindProperty("layers");
        
        // Start from layer 8 (first user-available layer)
        for (int i = 8; i < layers.arraySize; i++)
        {
            if (layers.GetArrayElementAtIndex(i).stringValue == "")
            {
                layers.GetArrayElementAtIndex(i).stringValue = layerName;
                tagManager.ApplyModifiedProperties();
                Debug.Log($"Added layer: {layerName} at index {i}");
                return;
            }
        }
        
        Debug.LogWarning($"Could not add layer {layerName}. All user layers are in use.");
    }
    
    public void OnPreprocessBuild(BuildReport report)
    {
        // Validate project settings before building
        ValidateProjectSettings();
    }
}
