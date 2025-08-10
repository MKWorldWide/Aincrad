using UdonSharp;
using UnityEngine;
using VRC.SDKBase;
using VRC.Udon;
using VRC.SDK3.Components;
using UnityEngine.Video;

[UdonBehaviourSyncMode(BehaviourSyncMode.None)]
public class PortalSystem : UdonSharpBehaviour
{
    [System.Serializable]
    public enum PortalType
    {
        WorldPortal,
        RenderTexture,
        VideoPanel
    }

    [System.Serializable]
    public class PortalData
    {
        public string portalName;
        public PortalType portalType;
        public Transform portalTransform;
        public string worldId; // For WorldPortal
        public Camera renderCamera; // For RenderTexture
        public VideoPlayer videoPlayer; // For VideoPanel
        public RenderTexture renderTexture; // For RenderTexture
        public Material screenMaterial; // Material that displays the portal
        public float autoCloseDelay = 60f; // Auto-close delay in seconds (0 = never)
        public AudioClip activationSound;
        public ParticleSystem activationEffect;
    }

    [Header("Portal Configuration")]
    public PortalData[] portals;
    public LayerMask portalLayer;
    public float interactionDistance = 2.5f;
    public KeyCode interactionKey = KeyCode.E;

    [Header("Performance Settings")]
    public int maxActivePortals = 2;
    public float portalUpdateInterval = 0.1f;

    private int activePortalCount = 0;
    private float lastUpdateTime;
    private VRCPlayerApi localPlayer;

    private void Start()
    {
        localPlayer = Networking.LocalPlayer;
        InitializePortals();
    }

    private void InitializePortals()
    {
        foreach (var portal in portals)
        {
            if (portal == null || portal.portalTransform == null) continue;

            switch (portal.portalType)
            {
                case PortalType.RenderTexture:
                    SetupRenderTexturePortal(portal);
                    break;
                case PortalType.VideoPanel:
                    SetupVideoPortal(portal);
                    break;
                case PortalType.WorldPortal:
                default:
                    // World portals are handled on interaction
                    break;
            }
        }
    }

    private void SetupRenderTexturePortal(PortalData portal)
    {
        if (portal.renderCamera != null && portal.renderTexture != null)
        {
            portal.renderCamera.targetTexture = portal.renderTexture;
            
            if (portal.screenMaterial != null)
            {
                portal.screenMaterial.mainTexture = portal.renderTexture;
            }
        }
    }

    private void SetupVideoPortal(PortalData portal)
    {
        if (portal.videoPlayer != null)
        {
            portal.videoPlayer.isLooping = true;
            portal.videoPlayer.playOnAwake = false;
            
            if (portal.renderTexture != null)
            {
                portal.videoPlayer.targetTexture = portal.renderTexture;
                
                if (portal.screenMaterial != null)
                {
                    portal.screenMaterial.mainTexture = portal.renderTexture;
                }
            }
        }
    }

    public override void Interact()
    {
        // Handle interaction with portals
        Ray ray = new Ray(transform.position, transform.forward);
        if (Physics.Raycast(ray, out RaycastHit hit, interactionDistance, portalLayer))
        {
            foreach (var portal in portals)
            {
                if (portal.portalTransform != null && hit.transform == portal.portalTransform)
                {
                    ActivatePortal(portal);
                    break;
                }
            }
        }
    }

    private void ActivatePortal(PortalData portal)
    {
        if (portal == null) return;

        // Play activation effects
        if (portal.activationSound != null)
        {
            AudioSource.PlayClipAtPoint(portal.activationSound, portal.portalTransform.position);
        }

        if (portal.activationEffect != null)
        {
            portal.activationEffect.Play();
        }

        switch (portal.portalType)
        {
            case PortalType.WorldPortal:
                ActivateWorldPortal(portal);
                break;
                
            case PortalType.RenderTexture:
                ToggleRenderTexturePortal(portal);
                break;
                
            case PortalType.VideoPanel:
                ToggleVideoPortal(portal);
                break;
        }

        // Handle portal limits and auto-close
        if (portal.autoCloseDelay > 0)
        {
            SendCustomEventDelayedSeconds(nameof(ClosePortal), portal.autoCloseDelay, VRC.Udon.Common.Enums.EventTiming.Update);
        }
    }

    private void ActivateWorldPortal(PortalData portal)
    {
        // Create and configure a VRC portal
        var portalObj = new GameObject($"PortalTo_{portal.worldId}");
        var portalMarker = portalObj.AddComponent<VRC_PortalMarker>();
        portalMarker.roomId = portal.worldId;
        portalObj.transform.SetPositionAndRotation(
            portal.portalTransform.position,
            portal.portalTransform.rotation
        );

        // Add collider and rigidbody for physics
        var collider = portalObj.AddComponent<BoxCollider>();
        collider.isTrigger = true;
        var rb = portalObj.AddComponent<Rigidbody>();
        rb.isKinematic = true;
    }

    private void ToggleRenderTexturePortal(PortalData portal)
    {
        if (portal.renderCamera != null)
        {
            bool isActive = !portal.renderCamera.gameObject.activeSelf;
            portal.renderCamera.gameObject.SetActive(isActive);
            
            if (isActive)
            {
                activePortalCount++;
                if (activePortalCount > maxActivePortals)
                {
                    // Deactivate the oldest active portal
                    // Implementation needed based on your tracking system
                }
            }
            else
            {
                activePortalCount = Mathf.Max(0, activePortalCount - 1);
            }
        }
    }

    private void ToggleVideoPortal(PortalData portal)
    {
        if (portal.videoPlayer != null)
        {
            if (portal.videoPlayer.isPlaying)
            {
                portal.videoPlayer.Pause();
                activePortalCount--;
            }
            else
            {
                portal.videoPlayer.Play();
                activePortalCount++;
                
                if (activePortalCount > maxActivePortals)
                {
                    // Pause the oldest playing video
                    // Implementation needed based on your tracking system
                }
            }
        }
    }

    public void ClosePortal(PortalData portal)
    {
        switch (portal.portalType)
        {
            case PortalType.RenderTexture:
                if (portal.renderCamera != null)
                {
                    portal.renderCamera.gameObject.SetActive(false);
                }
                break;
                
            case PortalType.VideoPanel:
                if (portal.videoPlayer != null)
                {
                    portal.videoPlayer.Stop();
                }
                break;
        }
        
        activePortalCount = Mathf.Max(0, activePortalCount - 1);
    }

    // Update is called once per frame
    private void Update()
    {
        // Only update at the specified interval for performance
        if (Time.time - lastUpdateTime < portalUpdateInterval) return;
        lastUpdateTime = Time.time;

        // Handle input for interaction
        if (Input.GetKeyDown(interactionKey))
        {
            Interact();
        }
    }

    // Cleanup when destroyed
    private void OnDestroy()
    {
        foreach (var portal in portals)
        {
            if (portal.renderTexture != null)
            portal.renderTexture.Release();
                
            if (portal.videoPlayer != null)
                portal.videoPlayer.Stop();
        }
    }
}
