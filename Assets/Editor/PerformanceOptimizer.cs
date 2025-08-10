using UnityEngine;
using UnityEditor;
using UnityEditor.SceneManagement;
using System.Collections.Generic;
using System.Linq;

public class PerformanceOptimizer : EditorWindow
{
    private bool optimizeMeshes = true;
    private bool optimizeTextures = true;
    private bool optimizeMaterials = true;
    private bool optimizeLights = true;
    private bool optimizeAudio = true;
    private bool optimizeParticles = true;
    
    [MenuItem("Aincrad/Optimize Scene")]
    public static void ShowWindow()
    {
        GetWindow<PerformanceOptimizer>("Performance Optimizer");
    }
    
    private void OnGUI()
    {
        GUILayout.Label("Performance Optimization", EditorStyles.boldLabel);
        EditorGUILayout.Space();
        
        optimizeMeshes = EditorGUILayout.Toggle("Optimize Meshes", optimizeMeshes);
        optimizeTextures = EditorGUILayout.Toggle("Optimize Textures", optimizeTextures);
        optimizeMaterials = EditorGUILayout.Toggle("Optimize Materials", optimizeMaterials);
        optimizeLights = EditorGUILayout.Toggle("Optimize Lights", optimizeLights);
        optimizeAudio = EditorGUILayout.Toggle("Optimize Audio", optimizeAudio);
        optimizeParticles = EditorGUILayout.Toggle("Optimize Particles", optimizeParticles);
        
        EditorGUILayout.Space();
        
        if (GUILayout.Button("Optimize Current Scene"))
        {
            OptimizeCurrentScene();
        }
        
        if (GUILayout.Button("Check Performance"))
        {
            CheckPerformance();
        }
    }
    
    private void OptimizeCurrentScene()
    {
        if (optimizeMeshes) OptimizeMeshes();
        if (optimizeTextures) OptimizeTextures();
        if (optimizeMaterials) OptimizeMaterials();
        if (optimizeLights) OptimizeLights();
        if (optimizeAudio) OptimizeAudio();
        if (optimizeParticles) OptimizeParticles();
        
        EditorSceneManager.MarkSceneDirty(EditorSceneManager.GetActiveScene());
        Debug.Log("Scene optimization complete!");
    }
    
    private void CheckPerformance()
    {
        int totalScore = 100;
        List<string> issues = new List<string>();
        
        // Check draw calls
        int drawCalls = UnityEditor.UnityStats.drawCalls;
        if (drawCalls > 100)
        {
            int deduction = Mathf.Min(20, (drawCalls - 100) / 10);
            totalScore -= deduction;
            issues.Add($"High draw calls: {drawCalls} (target < 100)");
        }
        
        // Check triangles
        int triangles = UnityEditor.UnityStats.triangles;
        if (triangles > 100000)
        {
            int deduction = Mathf.Min(20, (triangles - 100000) / 10000);
            totalScore -= deduction;
            issues.Add($"High triangle count: {triangles:N0} (target < 100,000)");
        }
        
        // Check textures
        var textures = Resources.FindObjectsOfTypeAll<Texture2D>();
        int largeTextures = textures.Count(t => t.width > 2048 || t.height > 2048);
        if (largeTextures > 0)
        {
            totalScore -= 10;
            issues.Add($"Found {largeTextures} textures larger than 2048x2048");
        }
        
        // Check lights
        var lights = FindObjectsOfType<Light>();
        int realtimeLights = lights.Count(l => l.lightmapBakeType != LightmapBakeType.Baked);
        if (realtimeLights > 1)
        {
            totalScore -= 10;
            issues.Add($"Found {realtimeLights} real-time lights (recommend 0-1)");
        }
        
        // Display results
        string message = $"Performance Score: {totalScore}/100\n\n";
        
        if (issues.Count > 0)
        {
            message += "Issues found:\n- " + string.Join("\n- ", issues);
        }
        else
        {
            message += "Great job! Your scene is well optimized.";
        }
        
        EditorUtility.DisplayDialog("Performance Check", message, "OK");
    }
    
    private void OptimizeMeshes()
    {
        // Add LOD groups to high-poly meshes
        var renderers = FindObjectsOfType<Renderer>();
        foreach (var renderer in renderers)
        {
            if (renderer.GetComponent<LODGroup>() == null && 
                renderer is MeshRenderer && 
                renderer.GetComponent<MeshFilter>() != null &&
                renderer.GetComponent<MeshFilter>().sharedMesh != null &&
                renderer.GetComponent<MeshFilter>().sharedMesh.vertexCount > 1000)
            {
                LODGroup lodGroup = renderer.gameObject.AddComponent<LODGroup>();
                
                // Create LOD levels
                LOD[] lods = new LOD[3];
                
                // High LOD (100%)
                Renderer[] renderersHigh = { renderer };
                lods[0] = new LOD(0.5f, renderersHigh);
                
                // Medium LOD (simplified mesh)
                // This would require a mesh simplification tool in a real implementation
                
                // Low LOD (billboard)
                // Create a simple quad as a billboard
                
                lodGroup.SetLODs(lods);
                lodGroup.RecalculateBounds();
            }
        }
        
        Debug.Log($"Optimized {renderers.Length} renderers");
    }
    
    private void OptimizeTextures()
    {
        // Set texture import settings
        string[] texturePaths = AssetDatabase.FindAssets("t:Texture2D")
            .Select(AssetDatabase.GUIDToAssetPath)
            .Where(p => !p.StartsWith("Assets/Plugins") && !p.Contains("Editor"))
            .ToArray();
        
        foreach (string path in texturePaths)
        {
            TextureImporter importer = AssetImporter.GetAtPath(path) as TextureImporter;
            if (importer != null)
            {
                bool modified = false;
                
                // Set texture type
                if (importer.textureType != TextureImporterType.Default)
                {
                    importer.textureType = TextureImporterType.Default;
                    modified = true;
                }
                
                // Set max size
                TextureImporterPlatformSettings settings = importer.GetPlatformTextureSettings("Android");
                if (settings.overridden == false || settings.maxTextureSize > 2048)
                {
                    settings.overridden = true;
                    settings.maxTextureSize = 2048;
                    settings.format = TextureImporterFormat.ASTC_6x6;
                    importer.SetPlatformTextureSettings(settings);
                    modified = true;
                }
                
                // Save changes
                if (modified)
                {
                    importer.SaveAndReimport();
                }
            }
        }
        
        Debug.Log($"Optimized {texturePaths.Length} textures");
    }
    
    private void OptimizeMaterials()
    {
        // Combine materials with the same shader and textures
        var materials = Resources.FindObjectsOfTypeAll<Material>();
        var materialGroups = materials
            .Where(m => m.shader != null)
            .GroupBy(m => new { m.shader.name, TextureSet = GetTextureSet(m) });
        
        foreach (var group in materialGroups)
        {
            if (group.Count() > 1)
            {
                Material firstMat = group.First();
                foreach (Material mat in group.Skip(1))
                {
                    // Replace all references to mat with firstMat
                    // This is a simplified example - in a real implementation, you'd need to handle
                    // all renderers and other objects that might reference these materials
                    var renderers = FindObjectsOfType<Renderer>();
                    foreach (var renderer in renderers)
                    {
                        for (int i = 0; i < renderer.sharedMaterials.Length; i++)
                        {
                            if (renderer.sharedMaterials[i] == mat)
                            {
                                var materialsArray = renderer.sharedMaterials;
                                materialsArray[i] = firstMat;
                                renderer.sharedMaterials = materialsArray;
                            }
                        }
                    }
                }
            }
        }
        
        Debug.Log("Optimized materials");
    }
    
    private string GetTextureSet(Material mat)
    {
        // Create a string that represents the set of textures used by this material
        var textureNames = mat.GetTexturePropertyNames()
            .Where(name => mat.GetTexture(name) != null)
            .OrderBy(name => name)
            .Select(name => $"{name}:{mat.GetTexture(name).name}");
            
        return string.Join("|", textureNames);
    }
    
    private void OptimizeLights()
    {
        // Convert real-time lights to baked where possible
        var lights = FindObjectsOfType<Light>();
        int bakedLights = 0;
        
        foreach (var light in lights)
        {
            // Skip UI and special case lights
            if (light.gameObject.CompareTag("UI") || light.type == LightType.Directional)
                continue;
                
            // If light doesn't move, bake it
            if (light.gameObject.isStatic)
            {
                light.lightmapBakeType = LightmapBakeType.Baked;
                bakedLights++;
            }
        }
        
        Debug.Log($"Optimized {bakedLights} lights (set to baked)");
    }
    
    private void OptimizeAudio()
    {
        // Set audio import settings
        string[] audioPaths = AssetDatabase.FindAssets("t:AudioClip")
            .Select(AssetDatabase.GUIDToAssetPath)
            .Where(p => !p.StartsWith("Assets/Plugins") && !p.Contains("Editor"))
            .ToArray();
            
        foreach (string path in audioPaths)
        {
            AudioImporter importer = AssetImporter.GetAtPath(path) as AudioImporter;
            if (importer != null)
            {
                bool modified = false;
                AudioImporterSampleSettings settings = importer.defaultSampleSettings;
                
                // Set compression format
                if (settings.compressionFormat != AudioCompressionFormat.Vorbis)
                {
                    settings.compressionFormat = AudioCompressionFormat.Vorbis;
                    modified = true;
                }
                
                // Set quality
                if (settings.quality > 0.5f)
                {
                    settings.quality = 0.5f; // Medium quality
                    modified = true;
                }
                
                // Apply settings if modified
                if (modified)
                {
                    importer.defaultSampleSettings = settings;
                    importer.SaveAndReimport();
                }
            }
        }
        
        Debug.Log($"Optimized {audioPaths.Length} audio clips");
    }
    
    private void OptimizeParticles()
    {
        var particleSystems = FindObjectsOfType<ParticleSystem>();
        
        foreach (var ps in particleSystems)
        {
            var main = ps.main;
            var emission = ps.emission;
            var shape = ps.shape;
            var collision = ps.collision;
            var renderer = ps.GetComponent<ParticleSystemRenderer>();
            
            // Optimize settings
            if (main.maxParticles > 1000)
                main.maxParticles = 1000;
                
            if (emission.rateOverTime.constant > 50)
                emission.rateOverTime = 50;
                
            if (renderer != null)
            {
                renderer.shadowCastingMode = UnityEngine.Rendering.ShadowCastingMode.Off;
                renderer.receiveShadows = false;
            }
        }
        
        Debug.Log($"Optimized {particleSystems.Length} particle systems");
    }
}
