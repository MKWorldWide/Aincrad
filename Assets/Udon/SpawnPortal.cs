using UdonSharp;
using VRC.SDKBase;
using VRC.Udon;
using VRC.SDK3.Components;
using UnityEngine;

public class SpawnPortal : UdonSharpBehaviour
{
    [Header("Portal Settings")]
    [Tooltip("The portal prefab to instantiate")]
    public GameObject portalPrefab;
    
    [Tooltip("Where to spawn the portal")]
    public Transform spawnPoint;
    
    [Tooltip("The world ID to link the portal to")]
    public string worldId = "wrld_00000000-0000-0000-0000-000000000000";
    
    [Header("Effects")]
    public ParticleSystem spawnEffect;
    public AudioSource spawnSound;
    
    [Header("Portal Lifetime")]
    [Tooltip("How long before the portal despawns (0 = never)")]
    public float portalLifetime = 60f;
    
    private GameObject currentPortal;
    
    public override void Interact()
    {
        if (portalPrefab == null || spawnPoint == null) 
        {
            Debug.LogError("Portal prefab or spawn point not assigned!");
            return;
        }
        
        // Remove existing portal if it exists
        if (currentPortal != null)
        {
            Destroy(currentPortal);
        }
        
        // Create new portal
        currentPortal = VRCInstantiate(portalPrefab);
        currentPortal.transform.SetPositionAndRotation(spawnPoint.position, spawnPoint.rotation);
        
        // Set up the portal
        var portalMarker = currentPortal.GetComponent<VRC_PortalMarker>();
        if (portalMarker != null)
        {
            portalMarker.roomId = worldId;
        }
        
        // Play effects
        if (spawnEffect) spawnEffect.Play();
        if (spawnSound) spawnSound.Play();
        
        // Set up auto-destruction if lifetime is greater than 0
        if (portalLifetime > 0)
        {
            Destroy(currentPortal, portalLifetime);
        }
    }
}
