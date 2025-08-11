using UnityEngine;
using VRC.SDKBase;
using VRC.Udon;
using VRC.Udon.Common.Interfaces;
using TMPro;
using UnityEngine.UI;

[UdonBehaviourSyncMode(BehaviourSyncMode.None)]
public class ModulePortal : UdonSharpBehaviour
{
    [Header("Portal Settings")]
    [Tooltip("Reference to the ModuleManager")]
    public ModuleManager moduleManager;
    
    [Header("UI References")]
    [Tooltip("Text element to display module name")]
    public TextMeshProUGUI moduleNameText;
    
    [Tooltip("Text element to display module description")]
    public TextMeshProUGUI moduleDescriptionText;
    
    [Tooltip("Image element to display module thumbnail")]
    public RawImage moduleThumbnail;
    
    [Tooltip("UI panel that appears when looking at the portal")]
    public GameObject infoPanel;
    
    [Header("Animation")]
    [Tooltip("Portal effect material")]
    public Material portalMaterial;
    
    [Tooltip("Rotation speed of the portal effect")]
    public float rotationSpeed = 10f;
    
    [Tooltip("Pulse speed of the portal effect")]
    public float pulseSpeed = 1f;
    
    [Tooltip("Intensity of the pulse effect")]
    public float pulseIntensity = 0.1f;
    
    // Module information
    [System.NonSerialized]
    public string targetModule = "";
    [System.NonSerialized]
    public string displayName = "";
    [System.NonSerialized]
    public string description = "";
    [System.NonSerialized]
    public Texture2D thumbnail;
    
    // Private variables
    private float pulseTimer = 0f;
    private bool isPlayerLooking = false;
    private VRCPlayerApi localPlayer;
    private const float LOOK_ANGLE_THRESHOLD = 30f;
    private const float LOOK_DISTANCE_THRESHOLD = 5f;
    
    private void Start()
    {
        localPlayer = Networking.LocalPlayer;
        
        // Initialize UI
        if (infoPanel != null)
            infoPanel.SetActive(false);
            
        // Initialize material
        if (portalMaterial != null)
        {
            portalMaterial = new Material(portalMaterial);
            GetComponent<Renderer>().material = portalMaterial;
        }
    }
    
    public void Initialize(string moduleId, string name, string desc, Texture2D thumb = null)
    {
        targetModule = moduleId;
        displayName = name;
        description = desc;
        thumbnail = thumb;
        
        // Update UI
        if (moduleNameText != null)
            moduleNameText.text = displayName;
            
        if (moduleDescriptionText != null)
            moduleDescriptionText.text = description;
            
        if (moduleThumbnail != null && thumbnail != null)
        {
            moduleThumbnail.texture = thumbnail;
        }
    }
    
    private void Update()
    {
        UpdatePortalEffect();
        
        // Check if player is looking at the portal
        if (localPlayer != null && localPlayer.isLocal)
        {
            CheckPlayerGaze();
        }
    }
    
    private void UpdatePortalEffect()
    {
        if (portalMaterial == null)
            return;
            
        // Rotate the portal texture
        float rotation = Time.time * rotationSpeed % 360f;
        portalMaterial.SetFloat("_Rotation", rotation);
        
        // Pulsing effect
        pulseTimer += Time.deltaTime * pulseSpeed;
        float pulse = Mathf.Sin(pulseTimer) * pulseIntensity + 1f;
        portalMaterial.SetFloat("_EmissionIntensity", pulse);
    }
    
    private void CheckPlayerGaze()
    {
        if (localPlayer == null || !localPlayer.IsValid())
            return;
            
        // Get player head position and forward direction
        Vector3 playerPos = localPlayer.GetTrackingData(VRCPlayerApi.TrackingDataType.Head).position;
        Vector3 playerForward = localPlayer.GetTrackingData(VRCPlayerApi.TrackingDataType.Head).rotation * Vector3.forward;
        
        // Direction from player to portal
        Vector3 toPortal = transform.position - playerPos;
        float distance = toPortal.magnitude;
        
        // Check if player is looking towards the portal
        float angle = Vector3.Angle(playerForward, toPortal.normalized);
        bool wasLooking = isPlayerLooking;
        isPlayerLooking = (angle < LOOK_ANGLE_THRESHOLD && distance < LOOK_DISTANCE_THRESHOLD);
        
        // Show/hide info panel
        if (isPlayerLooking != wasLooking && infoPanel != null)
        {
            infoPanel.SetActive(isPlayerLooking);
        }
    }
    
    // Called when player interacts with the portal
    public override void Interact()
    {
        if (moduleManager != null && !string.IsNullOrEmpty(targetModule))
        {
            moduleManager.SwitchToModule(targetModule);
        }
    }
    
    // Called when player enters the trigger
    public override void OnPlayerTriggerEnter(VRCPlayerApi player)
    {
        if (player.isLocal)
        {
            // Show UI or play effects when player is near the portal
        }
    }
    
    // Called when player exits the trigger
    public override void OnPlayerTriggerExit(VRCPlayerApi player)
    {
        if (player.isLocal && infoPanel != null)
        {
            infoPanel.SetActive(false);
        }
    }
    
    // Clean up material instance
    private void OnDestroy()
    {
        if (portalMaterial != null)
        {
            Destroy(portalMaterial);
        }
    }
    
    #if UNITY_EDITOR
    private void OnValidate()
    {
        // Update the portal effect in the editor
        if (portalMaterial != null && !Application.isPlaying)
        {
            var renderer = GetComponent<Renderer>();
            if (renderer != null)
            {
                if (Application.isEditor && !Application.isPlaying)
                {
                    // Create a temporary material instance for preview
                    var tempMaterial = new Material(portalMaterial);
                    renderer.material = tempMaterial;
                    
                    // Clean up the temporary material after a short delay
                    UnityEditor.EditorApplication.delayCall += () => {
                        if (renderer != null && renderer.sharedMaterial == tempMaterial)
                        {
                            renderer.sharedMaterial = portalMaterial;
                            DestroyImmediate(tempMaterial);
                        }
                    };
                }
            }
        }
    }
    #endif
}
