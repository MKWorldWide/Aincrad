using UdonSharp;
using VRC.SDKBase;
using VRC.Udon;
using UnityEngine;

public class GlassTeleport : UdonSharpBehaviour
{
    [Tooltip("The transform where the player will be teleported to")]
    public Transform destination;
    
    [Header("Visual Feedback")]
    public ParticleSystem teleportEffect;
    public AudioSource teleportSound;
    
    public override void Interact()
    {
        var localPlayer = Networking.LocalPlayer;
        if (localPlayer != null && destination != null)
        {
            // Play effects if assigned
            if (teleportEffect) teleportEffect.Play();
            if (teleportSound) teleportSound.Play();
            
            // Teleport the player
            localPlayer.TeleportTo(
                destination.position,
                destination.rotation,
                VRC_SceneDescriptor.SpawnOrientation.AlignPlayerWithSpawnPoint,
                true // lerp on remote
            );
        }
    }
}
