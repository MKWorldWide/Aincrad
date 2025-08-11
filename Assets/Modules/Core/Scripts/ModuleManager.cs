using UnityEngine;
using VRC.SDKBase;
using VRC.Udon;
using VRC.Udon.Common.Interfaces;
using System.Collections.Generic;
using System.Linq;

[UdonBehaviourSyncMode(BehaviourSyncMode.Managed)]
public class ModuleManager : UdonSharpBehaviour
{
    [Header("Module Configuration")]
    [Tooltip("List of all available world modules")]
    public ModuleInfo[] availableModules;
    
    [Header("Current Module")]
    [Tooltip("The current active module")]
    public string currentModule = "Hub";
    
    [Header("Portal Configuration")]
    [Tooltip("Prefab for portal to other modules")]
    public GameObject portalPrefab;
    
    [Tooltip("Where to spawn portals in the scene")]
    public Transform[] portalSpawnPoints;
    
    [Header("Performance")]
    [Tooltip("Unload unused assets after module change")]
    public bool unloadUnusedAssets = true;
    
    [Tooltip("Garbage collect after module change")]
    public bool garbageCollect = true;
    
    // Runtime data
    private Dictionary<string, ModuleInfo> moduleLookup;
    private Dictionary<string, GameObject> activePortals = new Dictionary<string, GameObject>();
    private VRCPlayerApi localPlayer;
    
    private void Start()
    {
        localPlayer = Networking.LocalPlayer;
        InitializeModuleSystem();
        SpawnModulePortals();
    }
    
    private void InitializeModuleSystem()
    {
        moduleLookup = new Dictionary<string, ModuleInfo>();
        
        foreach (var module in availableModules)
        {
            if (module != null && !string.IsNullOrEmpty(module.moduleName))
            {
                moduleLookup[module.moduleName] = module;
            }
        }
        
        Debug.Log($"[ModuleManager] Initialized with {moduleLookup.Count} modules");
    }
    
    private void SpawnModulePortals()
    {
        if (portalPrefab == null)
        {
            Debug.LogError("[ModuleManager] No portal prefab assigned!");
            return;
        }
        
        // Clear existing portals
        foreach (var portal in activePortals.Values)
        {
            if (portal != null)
            Destroy(portal);
        }
        activePortals.Clear();
        
        // Spawn portals for each module (except current one)
        int spawnIndex = 0;
        foreach (var module in availableModules)
        {
            if (module == null || module.moduleName == currentModule) 
                continue;
                
            if (spawnIndex >= portalSpawnPoints.Length)
            {
                Debug.LogWarning($"[ModuleManager] Not enough spawn points for all module portals. Need at least {moduleLookup.Count - 1}.");
                break;
            }
            
            // Create portal
            GameObject portal = VRCInstantiate(portalPrefab);
            portal.transform.SetPositionAndRotation(
                portalSpawnPoints[spawnIndex].position,
                portalSpawnPoints[spawnIndex].rotation
            );
            
            // Set up portal properties
            var portalController = portal.GetComponent<ModulePortal>();
            if (portalController != null)
            {
                portalController.Initialize(module.moduleName, module.displayName, module.description, module.thumbnail);
            }
            
            // Store reference
            activePortals[module.moduleName] = portal;
            spawnIndex++;
        }
    }
    
    public void SwitchToModule(string moduleName)
    {
        if (!moduleLookup.ContainsKey(moduleName))
        {
            Debug.LogError($"[ModuleManager] Module not found: {moduleName}");
            return;
        }
        
        if (moduleName == currentModule)
        {
            Debug.Log($"[ModuleManager] Already in module: {moduleName}");
            return;
        }
        
        Debug.Log($"[ModuleManager] Switching to module: {moduleName}");
        
        // Get the module info
        ModuleInfo targetModule = moduleLookup[moduleName];
        
        // Update current module
        string previousModule = currentModule;
        currentModule = moduleName;
        
        // Handle the module transition
        if (localPlayer != null && localPlayer.isLocal)
        {
            // For local testing, we'll just log the transition
            Debug.Log($"[ModuleManager] Would transition to: {targetModule.worldId}");
            
            // In a real build, you would use VRC_SceneDescriptor to transition
            // VRC_SceneDescriptor.Instance.TransitionToWorld(targetModule.worldId);
            
            // For now, we'll just update the UI to show the transition
            UpdateModuleUI(targetModule);
        }
        
        // Clean up
        if (unloadUnusedAssets)
            Resources.UnloadUnusedAssets();
            
        if (garbageCollect)
            System.GC.Collect();
    }
    
    private void UpdateModuleUI(ModuleInfo module)
    {
        // Find all ModuleUI components and update them
        var moduleUIs = FindObjectsOfType<ModuleUI>();
        foreach (var ui in moduleUIs)
        {
            ui.OnModuleChanged(module);
        }
    }
    
    // Editor-only functions for setup
    #if UNITY_EDITOR
    [ContextMenu("Find All Modules")]
    private void FindAllModules()
    {
        // Find all ModuleInfo assets in the project
        string[] guids = UnityEditor.AssetDatabase.FindAssets("t:ModuleInfo");
        
        List<ModuleInfo> modules = new List<ModuleInfo>();
        foreach (string guid in guids)
        {
            string path = UnityEditor.AssetDatabase.GUIDToAssetPath(guid);
            ModuleInfo module = UnityEditor.AssetDatabase.LoadAssetAtPath<ModuleInfo>(path);
            if (module != null)
                modules.Add(module);
        }
        
        availableModules = modules.ToArray();
        UnityEditor.EditorUtility.SetDirty(this);
        Debug.Log($"[ModuleManager] Found {modules.Count} modules in project");
    }
    #endif
}

[System.Serializable]
public class ModuleInfo : ScriptableObject
{
    [Header("Module Identification")]
    public string moduleName = "";
    public string displayName = "";
    [TextArea(3, 5)]
    public string description = "";
    public Texture2D thumbnail;
    
    [Header("World Configuration")]
    [Tooltip("VRChat World ID for this module")]
    public string worldId = "";
    
    [Tooltip("Is this module required to be downloaded?"
    public bool required = true;
    
    [Header("Performance Settings")]
    [Tooltip("Estimated memory usage in MB")]
    public int estimatedMemoryMB = 50;
    
    [Tooltip("Recommended minimum platform")]
    public Platform minPlatform = Platform.Quest;
    
    public enum Platform
    {
        PC,
        Quest,
        All
    }
    
    [Header("Dependencies")]
    [Tooltip("Other modules required by this module")]
    public string[] requiredModules;
    
    [Header("Spawn Points")]
    [Tooltip("Default spawn point for this module")]
    public Vector3 defaultSpawnPoint = Vector3.zero;
    
    [Tooltip("Default spawn rotation (euler angles)")]
    public Vector3 defaultSpawnRotation = Vector3.zero;
    
    // Add any module-specific data here
    
    #if UNITY_EDITOR
    [UnityEditor.MenuItem("Assets/Create/VRChat/Module Info")]
    public static void CreateModuleInfo()
    {
        ModuleInfo asset = CreateInstance<ModuleInfo>();
        
        string path = UnityEditor.AssetDatabase.GetAssetPath(UnityEditor.Selection.activeObject);
        if (path == "")
        {
            path = "Assets";
        }
        else if (System.IO.Path.GetExtension(path) != "")
        {
            path = path.Replace(System.IO.Path.GetFileName(UnityEditor.AssetDatabase.GetAssetPath(UnityEditor.Selection.activeObject)), "");
        }
        
        string assetPathAndName = UnityEditor.AssetDatabase.GenerateUniqueAssetPath(path + "/New Module.asset");
        
        UnityEditor.AssetDatabase.CreateAsset(asset, assetPathAndName);
        UnityEditor.AssetDatabase.SaveAssets();
        UnityEditor.EditorUtility.FocusProjectWindow();
        UnityEditor.Selection.activeObject = asset;
    }
    #endif
}
