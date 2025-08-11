using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine;
using UnityEngine.SceneManagement;
using VRC.SDK3.Components;
using VRC.SDK3.Editor;
using VRC.SDKBase;
using VRC.Udon;
using System.Collections.Generic;
using System.IO;

public class SceneSetup : EditorWindow
{
    private string worldName = "Glasswater Hub";
    private string author = "Your Name";
    private string description = "A beautiful hub world connecting various experiences";
    private int capacity = 16;
    private bool createSpawnPoint = true;
    private bool setupLighting = true;
    private bool setupAudio = true;
    private bool setupPortals = true;
    private Vector2 scrollPosition;

    [MenuItem("VRChat/Scene Setup")]
    public static void ShowWindow()
    {
        GetWindow<SceneSetup>("VRChat Scene Setup");
    }

    private void OnGUI()
    {
        scrollPosition = EditorGUILayout.BeginScrollView(scrollPosition);
        
        GUILayout.Label("VRChat World Setup", EditorStyles.boldLabel);
        EditorGUILayout.Space();

        worldName = EditorGUILayout.TextField("World Name", worldName);
        author = EditorGUILayout.TextField("Author", author);
        description = EditorGUILayout.TextField("Description", description);
        capacity = EditorGUILayout.IntField("Player Capacity", capacity);

        EditorGUILayout.Space();
        
        createSpawnPoint = EditorGUILayout.Toggle("Create Spawn Point", createSpawnPoint);
        setupLighting = EditorGUILayout.Toggle("Setup Lighting", setupLighting);
        setupAudio = EditorGUILayout.Toggle("Setup Audio", setupAudio);
        setupPortals = EditorGUILayout.Toggle("Setup Portals", setupPortals);

        EditorGUILayout.Space();

        if (GUILayout.Button("Setup Scene"))
        {
            SetupScene();
        }

        EditorGUILayout.EndScrollView();
    }

    private void SetupScene()
    {
        // Create a new scene or get the active scene
        Scene currentScene = SceneManager.GetActiveScene();
        if (currentScene == null || !currentScene.IsValid())
        {
            currentScene = EditorSceneManager.NewScene(NewSceneSetup.DefaultGameObjects, NewSceneMode.Single);
        }

        // Clear existing objects if needed
        if (currentScene.rootCount > 0)
        {
            if (!EditorUtility.DisplayDialog("Clear Scene?", 
                "The current scene is not empty. Do you want to clear it?", "Yes", "No"))
            {
                return;
            }
            
            // Clear the scene
            GameObject[] allObjects = currentScene.GetRootGameObjects();
            foreach (GameObject obj in allObjects)
            {
                DestroyImmediate(obj);
            }
        }

        // Create the scene root
        GameObject sceneRoot = new GameObject(worldName);
        GameObject environment = new GameObject("Environment");
        environment.transform.SetParent(sceneRoot.transform);

        // Set up VRC_SceneDescriptor
        SetupSceneDescriptor(sceneRoot);

        // Set up spawn point if requested
        if (createSpawnPoint)
        {
            SetupSpawnPoint(sceneRoot);
        }

        // Set up lighting if requested
        if (setupLighting)
        {
            SetupLighting(sceneRoot);
        }

        // Set up audio if requested
        if (setupAudio)
        {
            SetupAudio(sceneRoot);
        }

        // Set up portals if requested
        if (setupPortals)
        {
            SetupPortals(sceneRoot);
        }

        // Save the scene
        if (!string.IsNullOrEmpty(currentScene.path))
        {
            EditorSceneManager.MarkSceneDirty(currentScene);
            EditorSceneManager.SaveScene(currentScene);
        }
        else
        {
            string scenePath = EditorUtility.SaveFilePanel("Save Scene", "Assets/Scenes", worldName + ".unity", "unity");
            if (!string.IsNullOrEmpty(scenePath))
            {
                scenePath = "Assets" + scenePath.Replace(Application.dataPath, "");
                EditorSceneManager.SaveScene(currentScene, scenePath);
            }
        }

        Debug.Log("Scene setup complete!");
    }

    private void SetupSceneDescriptor(GameObject root)
    {
        GameObject descriptorObj = new GameObject("VRC_SceneDescriptor");
        descriptorObj.transform.SetParent(root.transform);
        
        VRC_SceneDescriptor descriptor = descriptorObj.AddComponent<VRC_SceneDescriptor>();
        descriptor.name = worldName;
        
        // Set up spawn points
        descriptor.spawns = new VRC_SceneDescriptor.SpawnList();
        
        // Set up respawn height
        descriptor.RespawnHeightY = -100f;
        
        // Set up player settings
        descriptor.ReleasePublic = true;
        
        // Set up world settings
        EditorUtility.SetDirty(descriptor);
    }

    private void SetupSpawnPoint(GameObject root)
    {
        GameObject spawnPoint = new GameObject("SpawnPoint");
        spawnPoint.transform.SetParent(root.transform);
        spawnPoint.transform.localPosition = Vector3.zero;
        spawnPoint.transform.rotation = Quaternion.identity;
        
        // Add spawn point component
        VRC_SpawnPoint spawnPointComponent = spawnPoint.AddComponent<VRC_SpawnPoint>();
        spawnPointComponent.allowOthersToUse = true;
        spawnPointComponent.PlayerSpawnOrientation = VRC_SceneDescriptor.SpawnOrientation.AlignPlayerWithSpawnPoint;
        
        // Make sure it's tagged as a spawn point
        spawnPoint.tag = "SpawnPoint";
        
        // Add a simple visual indicator
        GameObject spawnVisual = GameObject.CreatePrimitive(PrimitiveType.Cylinder);
        spawnVisual.name = "SpawnVisual";
        spawnVisual.transform.SetParent(spawnPoint.transform);
        spawnVisual.transform.localPosition = new Vector3(0f, 0.1f, 0f);
        spawnVisual.transform.localScale = new Vector3(1f, 0.1f, 1f);
        
        // Make it semi-transparent blue
        Renderer renderer = spawnVisual.GetComponent<Renderer>();
        if (renderer != null)
        {
            Material spawnMat = new Material(Shader.Find("Standard"));
            spawnMat.color = new Color(0f, 0.5f, 1f, 0.5f);
            renderer.material = spawnMat;
        }
    }

    private void SetupLighting(GameObject root)
    {
        // Create a directional light
        GameObject lightObj = new GameObject("Directional Light");
        lightObj.transform.SetParent(root.transform);
        lightObj.transform.rotation = Quaternion.Euler(50f, -30f, 0f);
        
        Light light = lightObj.AddComponent<Light>();
        light.type = LightType.Directional;
        light.intensity = 1f;
        light.shadows = LightShadows.Soft;
        
        // Set up light probe group
        GameObject lightProbeGroupObj = new GameObject("Light Probe Group");
        lightProbeGroupObj.transform.SetParent(root.transform);
        LightProbeGroup lightProbeGroup = lightProbeGroupObj.AddComponent<LightProbeGroup>();
        
        // Add a reflection probe
        GameObject reflectionProbeObj = new GameObject("Reflection Probe");
        reflectionProbeObj.transform.SetParent(root.transform);
        ReflectionProbe reflectionProbe = reflectionProbeObj.AddComponent<ReflectionProbe>();
        reflectionProbe.size = new Vector3(100f, 50f, 100f);
        reflectionProbe.mode = UnityEngine.Rendering.ReflectionProbeMode.Realtime;
        reflectionProbe.refreshMode = UnityEngine.Rendering.ReflectionProbeRefreshMode.ViaScripting;
        reflectionProbe.timeSlicingMode = UnityEngine.Rendering.ReflectionProbeTimeSlicingMode.IndividualFaces;
    }

    private void SetupAudio(GameObject root)
    {
        // Create an audio source for ambient sounds
        GameObject audioObj = new GameObject("Ambient Audio");
        audioObj.transform.SetParent(root.transform);
        
        AudioSource audioSource = audioObj.AddComponent<AudioSource>();
        audioSource.spatialBlend = 1f; // 3D sound
        audioSource.loop = true;
        audioSource.volume = 0.5f;
        audioSource.spread = 180f;
        audioSource.maxDistance = 50f;
        
        // Add audio reverb zone
        GameObject reverbZoneObj = new GameObject("Reverb Zone");
        reverbZoneObj.transform.SetParent(root.transform);
        reverbZoneObj.transform.localPosition = Vector3.zero;
        
        AudioReverbZone reverbZone = reverbZoneObj.AddComponent<AudioReverbZone>();
        reverbZone.minDistance = 5f;
        reverbZone.maxDistance = 50f;
        reverbZone.reverbPreset = AudioReverbPreset.Quarry;
    }

    private void SetupPortals(GameObject root)
    {
        // Create a portals container
        GameObject portalsObj = new GameObject("Portals");
        portalsObj.transform.SetParent(root.transform);
        
        // Add the portal system component if it exists
        if (TypeExists("PortalSystem"))
        {
            portalsObj.AddComponent(System.Type.GetType("PortalSystem, Assembly-CSharp"));
        }
        
        // Create a few example portal locations
        CreateExamplePortal(portalsObj, "Portal 1", new Vector3(5f, 0f, 0f));
        CreateExamplePortal(portalsObj, "Portal 2", new Vector3(-5f, 0f, 0f));
        CreateExamplePortal(portalsObj, "Portal 3", new Vector3(0f, 0f, 5f));
    }

    private void CreateExamplePortal(GameObject parent, string name, Vector3 position)
    {
        GameObject portalObj = new GameObject(name);
        portalObj.transform.SetParent(parent.transform);
        portalObj.transform.localPosition = position;
        
        // Add a quad for the portal surface
        GameObject portalSurface = GameObject.CreatePrimitive(PrimitiveType.Quad);
        portalSurface.name = "PortalSurface";
        portalSurface.transform.SetParent(portalObj.transform);
        portalSurface.transform.localPosition = Vector3.zero;
        portalSurface.transform.localRotation = Quaternion.Euler(0f, 180f, 0f);
        portalSurface.transform.localScale = new Vector3(2f, 3f, 1f);
        
        // Add a collider for interaction
        BoxCollider collider = portalSurface.AddComponent<BoxCollider>();
        collider.isTrigger = true;
        
        // Add UdonBehaviour for portal functionality
        UdonBehaviour udon = portalObj.AddComponent<UdonBehaviour>();
        
        // Add a simple text label
        GameObject labelObj = new GameObject("Label");
        labelObj.transform.SetParent(portalObj.transform);
        labelObj.transform.localPosition = new Vector3(0f, 2f, 0f);
        
        // Add TextMeshPro component if available
        if (TypeExists("TMPro.TextMeshPro"))
        {
            var textMesh = labelObj.AddComponent(System.Type.GetType("TMPro.TextMeshPro, Unity.TextMeshPro")) as Component;
            
            // Set text properties through reflection
            var textProperty = textMesh.GetType().GetProperty("text");
            if (textProperty != null)
            {
                textProperty.SetValue(textMesh, name);
            }
            
            var fontSizeProperty = textMesh.GetType().GetProperty("fontSize");
            if (fontSizeProperty != null)
            {
                fontSizeProperty.SetValue(textMesh, 0.5f);
            }
            
            var alignmentProperty = textMesh.GetType().GetProperty("alignment");
            if (alignmentProperty != null)
            {
                var alignmentType = System.Type.GetType("TMPro.TextAlignmentOptions, Unity.TextMeshPro");
                if (alignmentType != null)
                {
                    alignmentProperty.SetValue(textMesh, System.Enum.ToObject(alignmentType, 0x201)); // Center alignment
                }
            }
        }
    }

    private bool TypeExists(string typeName)
    {
        return System.Type.GetType(typeName) != null;
    }
}
