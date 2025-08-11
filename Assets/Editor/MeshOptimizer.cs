using UnityEditor;
using UnityEngine;
using System.Collections.Generic;
using System.Linq;
using System.IO;

public class MeshOptimizer : EditorWindow
{
    private string searchPath = "Assets/Models";
    private bool includeSubfolders = true;
    private bool generateLightmapUVs = true;
    private bool optimizeMesh = true;
    private bool generateColliders = true;
    private bool isReadable = false;
    private ModelImporterMeshCompression meshCompression = ModelImporterMeshCompression.Medium;
    private float scaleFactor = 1.0f;
    private bool showAdvanced = false;
    private Vector2 scrollPosition;
    private List<string> processedMeshes = new List<string>();
    private List<string> errorList = new List<string>();
    private int totalMeshes = 0;
    private int processedCount = 0;
    private bool isProcessing = false;

    [MenuItem("VRChat/Optimize Meshes")]
    public static void ShowWindow()
    {
        GetWindow<MeshOptimizer>("Mesh Optimizer");
    }

    private void OnGUI()
    {
        scrollPosition = EditorGUILayout.BeginScrollView(scrollPosition);
        
        GUILayout.Label("Mesh Optimization Tool", EditorStyles.boldLabel);
        EditorGUILayout.Space();

        // Search settings
        EditorGUILayout.LabelField("Search Settings", EditorStyles.boldLabel);
        searchPath = EditorGUILayout.TextField("Search Path", searchPath);
        includeSubfolders = EditorGUILayout.Toggle("Include Subfolders", includeSubfolders);
        
        EditorGUILayout.Space();
        
        // Mesh settings
        EditorGUILayout.LabelField("Mesh Settings", EditorStyles.boldLabel);
        optimizeMesh = EditorGUILayout.Toggle("Optimize Mesh", optimizeMesh);
        generateLightmapUVs = EditorGUILayout.Toggle("Generate Lightmap UVs", generateLightmapUVs);
        generateColliders = EditorGUILayout.Toggle("Generate Colliders", generateColliders);
        isReadable = EditorGUILayout.Toggle("Read/Write Enabled", isReadable);
        
        // Advanced settings
        showAdvanced = EditorGUILayout.Foldout(showAdvanced, "Advanced Settings");
        if (showAdvanced)
        {
            EditorGUI.indentLevel++;
            
            meshCompression = (ModelImporterMeshCompression)EditorGUILayout.EnumPopup("Mesh Compression", meshCompression);
            scaleFactor = EditorGUILayout.Slider("Scale Factor", scaleFactor, 0.1f, 10.0f);
            
            EditorGUI.indentLevel--;
        }
        
        EditorGUILayout.Space();
        
        // Process button
        if (GUILayout.Button("Optimize Meshes"))
        {
            ProcessMeshes();
        }
        
        // Progress bar
        if (isProcessing)
        {
            float progress = (float)processedCount / Mathf.Max(1, totalMeshes);
            Rect rect = EditorGUILayout.BeginVertical();
            EditorGUI.ProgressBar(rect, progress, $"Processing: {processedCount}/{totalMeshes} ({progress * 100:F1}%)");
            GUILayout.Space(18);
            EditorGUILayout.EndVertical();
        }
        
        // Show errors if any
        if (errorList.Count > 0)
        {
            EditorGUILayout.Space();
            EditorGUILayout.HelpBox($"Encountered {errorList.Count} errors during processing.", MessageType.Warning);
            
            if (GUILayout.Button("Show Errors"))
            {
                foreach (string error in errorList)
                    Debug.LogError(error);
            }
        }
        
        EditorGUILayout.EndScrollView();
    }
    
    private void ProcessMeshes()
    {
        // Get all mesh files in the specified directory
        string[] meshFiles = Directory.GetFiles(searchPath, "*.*", 
            includeSubfolders ? SearchOption.AllDirectories : SearchOption.TopDirectoryOnly)
            .Where(file => file.EndsWith(".fbx") || file.EndsWith(".obj") || file.EndsWith(".blend"))
            .ToArray();
        
        if (meshFiles.Length == 0)
        {
            Debug.LogWarning("No mesh files found in the specified path.");
            return;
        }
        
        totalMeshes = meshFiles.Length;
        processedCount = 0;
        isProcessing = true;
        errorList.Clear();
        processedMeshes.Clear();
        
        // Process each mesh
        foreach (string meshPath in meshFiles)
        {
            try
            {
                string assetPath = meshPath.Replace("\\", "/");
                ModelImporter importer = AssetImporter.GetAtPath(assetPath) as ModelImporter;
                
                if (importer != null)
                {
                    bool modified = false;
                    
                    // Set general settings
                    if (importer.meshCompression != meshCompression)
                    {
                        importer.meshCompression = meshCompression;
                        modified = true;
                    }
                    
                    if (importer.optimizeMesh != optimizeMesh)
                    {
                        importer.optimizeMesh = optimizeMesh;
                        modified = true;
                    }
                    
                    if (importer.isReadable != isReadable)
                    {
                        importer.isReadable = isReadable;
                        modified = true;
                    }
                    
                    if (importer.generateSecondaryUV != generateLightmapUVs)
                    {
                        importer.generateSecondaryUV = generateLightmapUVs;
                        modified = true;
                    }
                    
                    if (importer.addCollider != generateColliders)
                    {
                        importer.addCollider = generateColliders;
                        modified = true;
                    }
                    
                    if (Mathf.Abs(importer.globalScale - scaleFactor) > 0.001f)
                    {
                        importer.globalScale = scaleFactor;
                        modified = true;
                    }
                    
                    // Save the changes if modified
                    if (modified)
                    {
                        EditorUtility.SetDirty(importer);
                        importer.SaveAndReimport();
                        processedMeshes.Add(assetPath);
                    }
                }
            }
            catch (System.Exception e)
            {
                errorList.Add($"Error processing {meshPath}: {e.Message}");
            }
            
            processedCount++;
            EditorUtility.DisplayProgressBar("Optimizing Meshes", 
                $"Processing {processedCount}/{totalMeshes}: {Path.GetFileName(meshPath)}", 
                (float)processedCount / totalMeshes);
        }
        
        // Clean up
        EditorUtility.ClearProgressBar();
        isProcessing = false;
        AssetDatabase.Refresh();
        
        // Show results
        string result = $"Optimization complete!\nProcessed: {processedMeshes.Count} meshes";
        if (errorList.Count > 0)
            result += $"\nErrors: {errorList.Count}";
        
        EditorUtility.DisplayDialog("Mesh Optimization", result, "OK");
    }
    
    // Static method for programmatic access
    public static void OptimizeAllMeshes()
    {
        var optimizer = CreateInstance<MeshOptimizer>();
        optimizer.ProcessMeshes();
    }
}
