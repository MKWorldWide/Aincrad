using UnityEditor;
using UnityEngine;
using System.IO;
using System.Collections.Generic;
using System.Linq;

public class TextureOptimizer : EditorWindow
{
    private string searchPath = "Assets/Textures";
    private bool includeSubfolders = true;
    private int maxTextureSize = 2048;
    private TextureImporterFormat androidFormat = TextureImporterFormat.ASTC_6x6;
    private TextureImporterFormat standaloneFormat = TextureImporterFormat.BC7;
    private bool generateMipMaps = true;
    private bool isReadable = false;
    private bool sRGB = true;
    private bool compressOnImport = true;
    private bool showAdvanced = false;
    private Vector2 scrollPosition;
    private List<string> processedTextures = new List<string>();
    private List<string> errorList = new List<string>();
    private int totalTextures = 0;
    private int processedCount = 0;
    private bool isProcessing = false;

    [MenuItem("VRChat/Optimize Textures")]
    public static void ShowWindow()
    {
        GetWindow<TextureOptimizer>("Texture Optimizer");
    }

    private void OnGUI()
    {
        scrollPosition = EditorGUILayout.BeginScrollView(scrollPosition);
        
        GUILayout.Label("Texture Optimization Tool", EditorStyles.boldLabel);
        EditorGUILayout.Space();

        // Search settings
        EditorGUILayout.LabelField("Search Settings", EditorStyles.boldLabel);
        searchPath = EditorGUILayout.TextField("Search Path", searchPath);
        includeSubfolders = EditorGUILayout.Toggle("Include Subfolders", includeSubfolders);
        
        EditorGUILayout.Space();
        
        // Texture settings
        EditorGUILayout.LabelField("Texture Settings", EditorStyles.boldLabel);
        maxTextureSize = EditorGUILayout.IntSlider("Max Texture Size", maxTextureSize, 32, 8192);
        generateMipMaps = EditorGUILayout.Toggle("Generate Mip Maps", generateMipMaps);
        isReadable = EditorGUILayout.Toggle("Read/Write Enabled", isReadable);
        sRGB = EditorGUILayout.Toggle("sRGB", sRGB);
        
        // Platform-specific settings
        showAdvanced = EditorGUILayout.Foldout(showAdvanced, "Advanced Settings");
        if (showAdvanced)
        {
            EditorGUI.indentLevel++;
            
            EditorGUILayout.Space();
            EditorGUILayout.LabelField("Android (Quest)", EditorStyles.boldLabel);
            androidFormat = (TextureImporterFormat)EditorGUILayout.EnumPopup("Android Format", androidFormat);
            
            EditorGUILayout.Space();
            EditorGUILayout.LabelField("Standalone (PC)", EditorStyles.boldLabel);
            standaloneFormat = (TextureImporterFormat)EditorGUILayout.EnumPopup("Standalone Format", standaloneFormat);
            
            EditorGUILayout.Space();
            compressOnImport = EditorGUILayout.Toggle("Compress on Import", compressOnImport);
            
            EditorGUI.indentLevel--;
        }
        
        EditorGUILayout.Space();
        
        // Process button
        if (GUILayout.Button("Optimize Textures"))
        {
            ProcessTextures();
        }
        
        // Progress bar
        if (isProcessing)
        {
            float progress = (float)processedCount / Mathf.Max(1, totalTextures);
            Rect rect = EditorGUILayout.BeginVertical();
            EditorGUI.ProgressBar(rect, progress, $"Processing: {processedCount}/{totalTextures} ({progress * 100:F1}%)");
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
    
    private void ProcessTextures()
    {
        // Get all texture files in the specified directory
        string[] textureFiles = Directory.GetFiles(searchPath, "*.*", 
            includeSubfolders ? SearchOption.AllDirectories : SearchOption.TopDirectoryOnly)
            .Where(file => file.EndsWith(".png") || file.EndsWith(".jpg") || file.EndsWith(".jpeg") || file.EndsWith(".tga") || file.EndsWith(".tif") || file.EndsWith(".tiff"))
            .ToArray();
        
        if (textureFiles.Length == 0)
        {
            Debug.LogWarning("No texture files found in the specified path.");
            return;
        }
        
        totalTextures = textureFiles.Length;
        processedCount = 0;
        isProcessing = true;
        errorList.Clear();
        
        // Process each texture
        foreach (string texturePath in textureFiles)
        {
            try
            {
                string assetPath = texturePath.Replace("\\", "/");
                TextureImporter importer = AssetImporter.GetAtPath(assetPath) as TextureImporter;
                
                if (importer != null)
                {
                    // Set platform settings
                    TextureImporterPlatformSettings androidSettings = importer.GetPlatformTextureSettings("Android");
                    androidSettings.overridden = true;
                    androidSettings.maxTextureSize = maxTextureSize;
                    androidSettings.format = androidFormat;
                    importer.SetPlatformTextureSettings(androidSettings);
                    
                    TextureImporterPlatformSettings standaloneSettings = importer.GetPlatformTextureSettings("Standalone");
                    standaloneSettings.overridden = true;
                    standaloneSettings.maxTextureSize = maxTextureSize;
                    standaloneSettings.format = standaloneFormat;
                    importer.SetPlatformTextureSettings(standaloneSettings);
                    
                    // Set general settings
                    importer.textureType = TextureImporterType.Default;
                    importer.sRGBTexture = sRGB;
                    importer.alphaSource = TextureImporterAlphaSource.FromInput;
                    importer.alphaIsTransparency = true;
                    importer.mipmapEnabled = generateMipMaps;
                    importer.isReadable = isReadable;
                    importer.streamingMipmaps = true;
                    importer.mipmapFilter = TextureImporterMipFilter.KaiserFilter;
                    importer.wrapMode = TextureWrapMode.Repeat;
                    importer.filterMode = FilterMode.Bilinear;
                    importer.anisoLevel = 1;
                    importer.npotScale = TextureImporterNPOTScale.ToNearest;
                    importer.compressionQuality = 100;
                    
                    // Save the changes
                    EditorUtility.SetDirty(importer);
                    importer.SaveAndReimport();
                    
                    processedTextures.Add(assetPath);
                }
            }
            catch (System.Exception e)
            {
                errorList.Add($"Error processing {texturePath}: {e.Message}");
            }
            
            processedCount++;
            EditorUtility.DisplayProgressBar("Optimizing Textures", 
                $"Processing {processedCount}/{totalTextures}: {Path.GetFileName(texturePath)}", 
                (float)processedCount / totalTextures);
        }
        
        // Clean up
        EditorUtility.ClearProgressBar();
        isProcessing = false;
        AssetDatabase.Refresh();
        
        // Show results
        string result = $"Optimization complete!\nProcessed: {processedTextures.Count} textures";
        if (errorList.Count > 0)
            result += $"\nErrors: {errorList.Count}";
        
        EditorUtility.DisplayDialog("Texture Optimization", result, "OK");
    }
}
