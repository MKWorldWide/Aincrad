using UdonSharp;
using UnityEngine;
using VRC.SDKBase;
using VRC.Udon;
using VRC.Udon.Common.Interfaces;
using System.Collections.Generic;

[UdonBehaviourSyncMode(BehaviourSyncMode.Managed)]
public class VRWorldManager : UdonSharpBehaviour
{
    [Header("World Settings")]
    public string worldName = "Glasswater Hub";
    public string author = "Your Name";
    [TextArea(3, 10)]
    public string description = "A beautiful hub world connecting various experiences";
    public string version = "1.0.0";
    public int maxPlayers = 16;
    public int recommendedPlayers = 8;
    public string[] tags = new string[] { "hub", "social", "exploration" };

    [Header("Performance Settings")]
    public bool enablePerformanceMode = false;
    public int maxDrawCalls = 100;
    public int maxPolyCount = 100000;
    public int maxTextureSize = 2048;
    public bool enableMobileOptimizations = true;

    [Header("References")]
    public Transform[] spawnPoints;
    public GameObject[] objectsToDisableInVR;
    public GameObject[] objectsToEnableInVR;
    public Material[] materialsToOptimize;

    [Header("Debug")]
    public bool showDebugInfo = false;
    public TextMesh debugText;

    private VRCPlayerApi localPlayer;
    private int currentSpawnIndex = 0;
    private float lastPerformanceCheck = 0f;
    private const float PERFORMANCE_CHECK_INTERVAL = 5f;

    private void Start()
    {
        localPlayer = Networking.LocalPlayer;
        
        // Initialize world settings
        InitializeWorld();
        
        // Set up spawn points
        if (spawnPoints == null || spawnPoints.Length == 0)
        {
            Debug.LogWarning("No spawn points assigned. Using default spawn at origin.");
            spawnPoints = new Transform[] { transform };
        }
        
        // Handle VR-specific setup
        if (localPlayer != null && localPlayer.IsUserInVR())
        {
            foreach (var obj in objectsToDisableInVR)
            {
                if (obj != null) obj.SetActive(false);
            }
            
            foreach (var obj in objectsToEnableInVR)
            {
                if (obj != null) obj.SetActive(true);
            }
        }
        
        // Initial performance optimization
        if (enablePerformanceMode)
        {
            OptimizePerformance();
        }
    }

    private void Update()
    {
        // Periodically check performance
        if (Time.time - lastPerformanceCheck > PERFORMANCE_CHECK_INTERVAL)
        {
            lastPerformanceCheck = Time.time;
            CheckPerformance();
            
            if (showDebugInfo && debugText != null)
            {
                UpdateDebugInfo();
            }
        }
    }

    private void InitializeWorld()
    {
        // Set up any world-specific initialization here
        Debug.Log($"Initializing {worldName} v{version} by {author}");
        Debug.Log($"Description: {description}");
        
        // Apply mobile optimizations if needed
        if (enableMobileOptimizations && IsMobilePlatform())
        {
            ApplyMobileOptimizations();
        }
        
        // Optimize materials
        OptimizeMaterials();
    }

    private void OptimizePerformance()
    {
        // Apply performance optimizations
        QualitySettings.vSyncCount = 0;
        Application.targetFrameRate = 90;
        
        // Optimize shadows
        QualitySettings.shadowDistance = 30f;
        QualitySettings.shadowResolution = ShadowResolution.Low;
        QualitySettings.shadowCascades = 1;
        
        // Optimize particles
        var particleSystems = FindObjectsOfType<ParticleSystem>();
        foreach (var ps in particleSystems)
        {
            var main = ps.main;
            main.maxParticles = Mathf.Min(main.maxParticles, 100);
        }
        
        Debug.Log($"Applied performance optimizations to {particleSystems.Length} particle systems");
    }

    private void OptimizeMaterials()
    {
        // Optimize materials for performance
        foreach (var mat in materialsToOptimize)
        {
            if (mat == null) continue;
            
            // Disable GPU instancing on mobile
            if (IsMobilePlatform())
            {
                mat.enableInstancing = false;
            }
            
            // Use simpler shaders on mobile
            if (IsMobilePlatform() && mat.shader.name.Contains("Standard"))
            {
                mat.shader = Shader.Find("Mobile/Diffuse");
            }
        }
        
        Debug.Log($"Optimized {materialsToOptimize.Length} materials");
    }

    private void CheckPerformance()
    {
        int currentDrawCalls = UnityStats.drawCalls;
        int currentTriangles = UnityStats.triangles;
        float fps = 1f / Time.deltaTime;
        
        if (currentDrawCalls > maxDrawCalls * 0.8f)
        {
            Debug.LogWarning($"High draw calls: {currentDrawCalls}/{maxDrawCalls}");
        }
        
        if (currentTriangles > maxPolyCount * 0.8f)
        {
            Debug.LogWarning($"High triangle count: {currentTriangles}/{maxPolyCount}");
        }
        
        if (fps < 45f)
        {
            Debug.LogWarning($"Low FPS: {fps:F1}");
            
            // If FPS is critically low, apply more aggressive optimizations
            if (fps < 30f && enablePerformanceMode)
            {
                ApplyEmergencyOptimizations();
            }
        }
    }

    private void ApplyEmergencyOptimizations()
    {
        Debug.Log("Applying emergency optimizations...");
        
        // Reduce quality settings
        QualitySettings.shadowDistance = 10f;
        QualitySettings.pixelLightCount = 0;
        
        // Disable non-essential objects
        var renderers = FindObjectsOfType<Renderer>();
        foreach (var renderer in renderers)
        {
            // Skip important objects
            if (renderer.CompareTag("Essential")) continue;
            
            // Disable objects that are far from the player
            if (Vector3.Distance(renderer.transform.position, localPlayer.GetPosition()) > 20f)
            {
                renderer.enabled = false;
            }
        }
    }

    private void ApplyMobileOptimizations()
    {
        Debug.Log("Applying mobile optimizations...");
        
        // Reduce texture quality
        QualitySettings.globalTextureMipmapLimit = 1;
        
        // Disable real-time shadows
        QualitySettings.shadows = ShadowQuality.Disable;
        
        // Reduce particle limits
        var particleSystems = FindObjectsOfType<ParticleSystem>();
        foreach (var ps in particleSystems)
        {
            var main = ps.main;
            main.maxParticles = Mathf.Min(main.maxParticles, 50);
        }
    }

    private void UpdateDebugInfo()
    {
        if (debugText == null) return;
        
        int currentDrawCalls = UnityStats.drawCalls;
        int currentTriangles = UnityStats.triangles;
        float fps = 1f / Time.deltaTime;
        
        string debugInfo = $"""
        {worldName} v{version}
        FPS: {fps:F1}
        Draw Calls: {currentDrawCalls}/{maxDrawCalls}
        Triangles: {currentTriangles}/{maxPolyCount}
        Players: {VRCPlayerApi.GetPlayerCount()}
        Platform: {(IsMobilePlatform() ? "Mobile" : "PC")}
        """;
        
        debugText.text = debugInfo;
    }

    public override void OnPlayerJoined(VRCPlayerApi player)
    {
        Debug.Log($"Player joined: {player.displayName}");
        
        // Update player count display if needed
        if (debugText != null)
        {
            UpdateDebugInfo();
        }
    }

    public override void OnPlayerLeft(VRCPlayerApi player)
    {
        Debug.Log($"Player left: {player.displayName}");
        
        // Update player count display if needed
        if (debugText != null)
        {
            UpdateDebugInfo();
        }
    }

    public void TeleportToSpawn(VRCPlayerApi player = null)
    {
        if (player == null) player = localPlayer;
        if (player == null || spawnPoints == null || spawnPoints.Length == 0) return;
        
        // Cycle through spawn points
        currentSpawnIndex = (currentSpawnIndex + 1) % spawnPoints.Length;
        Transform spawnPoint = spawnPoints[currentSpawnIndex];
        
        // Teleport the player
        player.TeleportTo(
            spawnPoint.position,
            spawnPoint.rotation,
            VRC_SceneDescriptor.SpawnOrientation.AlignPlayerWithSpawnPoint,
            true
        );
    }

    public void ToggleDebugInfo()
    {
        showDebugInfo = !showDebugInfo;
        
        if (debugText != null)
        {
            debugText.gameObject.SetActive(showDebugInfo);
            
            if (showDebugInfo)
            {
                UpdateDebugInfo();
            }
        }
    }

    private bool IsMobilePlatform()
    {
        return Application.platform == RuntimePlatform.Android || 
               Application.platform == RuntimePlatform.IPhonePlayer;
    }

    // Editor-only functions
    #if UNITY_EDITOR
    [ContextMenu("Find All Spawn Points")]
    private void FindAllSpawnPoints()
    {
        var spawns = GameObject.FindGameObjectsWithTag("SpawnPoint");
        spawnPoints = new Transform[spawns.Length];
        
        for (int i = 0; i < spawns.Length; i++)
        {
            spawnPoints[i] = spawns[i].transform;
        }
        
        Debug.Log($"Found {spawnPoints.Length} spawn points");
        UnityEditor.EditorUtility.SetDirty(this);
    }
    
    [ContextMenu("Find All Materials")]
    private void FindAllMaterials()
    {
        var materials = new List<Material>();
        var renderers = FindObjectsOfType<Renderer>();
        
        foreach (var renderer in renderers)
        {
            foreach (var mat in renderer.sharedMaterials)
            {
                if (mat != null && !materials.Contains(mat))
                {
                    materials.Add(mat);
                }
            }
        }
        
        materialsToOptimize = materials.ToArray();
        Debug.Log($"Found {materialsToOptimize.Length} unique materials");
        UnityEditor.EditorUtility.SetDirty(this);
    }
    #endif
}
