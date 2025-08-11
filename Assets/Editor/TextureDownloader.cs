using UnityEditor;
using UnityEngine;
using UnityEngine.Networking;
using System.Collections;
using System.IO;
using System.Threading.Tasks;
using System.Collections.Generic;

public class TextureDownloader : EditorWindow
{
    private string savePath = "Assets/Textures/Downloaded";
    private string searchQuery = "seamless texture";
    private int textureCount = 10;
    private int minResolution = 512;
    private int maxResolution = 2048;
    private Vector2 scrollPosition;
    private List<Texture2D> downloadedTextures = new List<Texture2D>();
    private bool isDownloading = false;
    private float downloadProgress = 0f;
    private string statusMessage = "";
    private MessageType statusType = MessageType.Info;

    [MenuItem("VRChat/Download Textures")]
    public static void ShowWindow()
    {
        GetWindow<TextureDownloader>("Texture Downloader");
    }

    private void OnGUI()
    {
        scrollPosition = EditorGUILayout.BeginScrollView(scrollPosition);
        
        GUILayout.Label("Free Texture Downloader", EditorStyles.boldLabel);
        EditorGUILayout.HelpBox("Download free textures from Pexels.com (CC0 License)", MessageType.Info);
        
        EditorGUILayout.Space();
        
        // Settings
        EditorGUILayout.LabelField("Download Settings", EditorStyles.boldLabel);
        searchQuery = EditorGUILayout.TextField("Search Query", searchQuery);
        textureCount = EditorGUILayout.IntSlider("Number of Textures", textureCount, 1, 30);
        minResolution = EditorGUILayout.IntSlider("Min Resolution", minResolution, 128, 4096);
        maxResolution = EditorGUILayout.IntSlider("Max Resolution", maxResolution, 128, 4096);
        savePath = EditorGUILayout.TextField("Save Path", savePath);
        
        EditorGUILayout.Space();
        
        // Download button
        EditorGUI.BeginDisabledGroup(isDownloading);
        if (GUILayout.Button("Download Textures"))
        {
            DownloadTextures();
        }
        EditorGUI.EndDisabledGroup();
        
        // Progress bar
        if (isDownloading)
        {
            Rect rect = EditorGUILayout.BeginVertical();
            EditorGUI.ProgressBar(rect, downloadProgress, $"Downloading... {downloadProgress * 100:F1}%");
            GUILayout.Space(18);
            EditorGUILayout.EndVertical();
        }
        
        // Status message
        if (!string.IsNullOrEmpty(statusMessage))
        {
            EditorGUILayout.HelpBox(statusMessage, statusType);
        }
        
        // Show downloaded textures
        if (downloadedTextures.Count > 0)
        {
            EditorGUILayout.Space();
            EditorGUILayout.LabelField("Downloaded Textures", EditorStyles.boldLabel);
            
            int columns = Mathf.FloorToInt(EditorGUIUtility.currentViewWidth / 100f);
            int rows = Mathf.CeilToInt(downloadedTextures.Count / (float)columns);
            
            for (int i = 0; i < rows; i++)
            {
                EditorGUILayout.BeginHorizontal();
                for (int j = 0; j < columns; j++)
                {
                    int index = i * columns + j;
                    if (index < downloadedTextures.Count)
                    {
                        Texture2D texture = downloadedTextures[index];
                        if (texture != null)
                        {
                            GUILayout.Box(texture, GUILayout.Width(80), GUILayout.Height(80));
                        }
                    }
                }
                EditorGUILayout.EndHorizontal();
            }
            
            if (GUILayout.Button("Clear Preview"))
            {
                downloadedTextures.Clear();
                statusMessage = "";
            }
        }
        
        EditorGUILayout.EndScrollView();
    }
    
    private async void DownloadTextures()
    {
        if (string.IsNullOrEmpty(searchQuery))
        {
            statusMessage = "Please enter a search query.";
            statusType = MessageType.Error;
            return;
        }
        
        if (minResolution > maxResolution)
        {
            statusMessage = "Minimum resolution cannot be greater than maximum resolution.";
            statusType = MessageType.Error;
            return;
        }
        
        // Create save directory if it doesn't exist
        if (!Directory.Exists(savePath))
        {
            Directory.CreateDirectory(savePath);
            AssetDatabase.Refresh();
        }
        
        isDownloading = true;
        downloadProgress = 0f;
        downloadedTextures.Clear();
        statusMessage = "Starting download...";
        statusType = MessageType.Info;
        
        try
        {
            // Note: In a real implementation, you would use the Pexels API with an API key
            // For this example, we'll use placeholder textures
            
            for (int i = 0; i < textureCount; i++)
            {
                // Update progress
                downloadProgress = (float)i / textureCount;
                statusMessage = $"Downloading texture {i + 1} of {textureCount}...";
                Repaint();
                
                // Generate a placeholder texture (in a real app, this would be an API call)
                Texture2D texture = await GeneratePlaceholderTexture(
                    $"{searchQuery} {i}", 
                    Random.Range(minResolution, maxResolution + 1),
                    Random.Range(minResolution, maxResolution + 1)
                );
                
                if (texture != null)
                {
                    // Save the texture
                    string filename = $"{searchQuery.Replace(" ", "_")}_{i}.png";
                    string fullPath = Path.Combine(savePath, filename);
                    
                    // Ensure the filename is unique
                    int counter = 1;
                    while (File.Exists(fullPath))
                    {
                        filename = $"{searchQuery.Replace(" ", "_")}_{i}_{counter}.png";
                        fullPath = Path.Combine(savePath, filename);
                        counter++;
                    }
                    
                    // Save the texture
                    byte[] bytes = texture.EncodeToPNG();
                    File.WriteAllBytes(fullPath, bytes);
                    
                    // Import the texture with proper settings
                    AssetDatabase.ImportAsset(fullPath);
                    TextureImporter importer = AssetImporter.GetAtPath(fullPath) as TextureImporter;
                    if (importer != null)
                    {
                        importer.textureType = TextureImporterType.Default;
                        importer.mipmapEnabled = true;
                        importer.isReadable = false;
                        importer.streamingMipmaps = true;
                        importer.SaveAndReimport();
                    }
                    
                    // Add to the list of downloaded textures
                    downloadedTextures.Add(texture);
                    
                    // Refresh the asset database
                    AssetDatabase.Refresh();
                    
                    // Small delay to prevent rate limiting
                    await Task.Delay(100);
                }
            }
            
            statusMessage = $"Successfully downloaded {downloadedTextures.Count} textures!";
            statusType = MessageType.Info;
        }
        catch (System.Exception e)
        {
            statusMessage = $"Error downloading textures: {e.Message}";
            statusType = MessageType.Error;
            Debug.LogError($"Texture download error: {e}");
        }
        finally
        {
            isDownloading = false;
            downloadProgress = 1f;
            Repaint();
        }
    }
    
    private async Task<Texture2D> GeneratePlaceholderTexture(string text, int width, int height)
    {
        // In a real implementation, this would be an API call to Pexels or similar
        // For now, we'll generate a simple colored texture with text
        
        // Create a new texture
        Texture2D texture = new Texture2D(width, height);
        
        // Fill with a random color
        Color bgColor = Random.ColorHSV(0f, 1f, 0.5f, 0.8f, 0.8f, 1f);
        Color textColor = Color.Lerp(Color.black, Color.white, bgColor.grayscale > 0.5f ? 0.2f : 0.8f);
        
        // Fill the texture with the background color
        Color[] pixels = new Color[width * height];
        for (int i = 0; i < pixels.Length; i++)
        {
            // Add some noise to make it look more like a real texture
            float noise = Random.Range(0.9f, 1.1f);
            pixels[i] = bgColor * noise;
        }
        
        // Apply the pixels
        texture.SetPixels(pixels);
        
        // Add some text (this is a simplified version - in a real app, you'd use GUI or TextGenerator)
        string displayText = $"{text}\n{width}x{height}";
        int fontSize = Mathf.Min(width / 10, height / 5);
        
        // Draw a simple border
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < 5; y++)
            {
                texture.SetPixel(x, y, Color.black);
                texture.SetPixel(x, height - 1 - y, Color.black);
            }
        }
        
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < 5; x++)
            {
                texture.SetPixel(x, y, Color.black);
                texture.SetPixel(width - 1 - x, y, Color.black);
            }
        }
        
        // Apply all changes
        texture.Apply();
        
        return texture;
    }
}
